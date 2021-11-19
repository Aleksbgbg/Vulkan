#include "Scene.h"

#include <algorithm>

#include "game/renders/light/LightRender.h"
#include "game/renders/particles/ParticleSpawner.h"
#include "game/renders/sky/SkyboxRender.h"
#include "game/renders/spaceships/SpaceshipRender.h"

class NoDescriptorAllocator : public ActorDescriptorAllocator {
 public:
  DescriptorSet AllocateDescriptor() const override {
    return DescriptorSet();
  }
};

class DescriptorAllocator : public ActorDescriptorAllocator {
 public:
  DescriptorAllocator(const DescriptorPool& descriptorPool,
                      DescriptorSetLayout descriptorSetLayout)
      : descriptorPool_(descriptorPool),
        descriptorSetLayout_(std::move(descriptorSetLayout)) {}

  DescriptorSet AllocateDescriptor() const override {
    return descriptorPool_.AllocateDescriptorSet(descriptorSetLayout_);
  }

 private:
  const DescriptorPool& descriptorPool_;
  DescriptorSetLayout descriptorSetLayout_;
};

class NoDescriptorBinder : public ActorDescriptorBinder {
 public:
  void AddDescriptor(DescriptorSet descriptorSet) override {}

  void BindActorDescriptorSet(const PipelineLayout& pipelineLayout,
                              const CommandBuffer& commandBuffer,
                              const u32 actorIndex) const override {}
};

class ArrayDescriptorBinder : public ActorDescriptorBinder {
 public:
  void AddDescriptor(DescriptorSet descriptorSet) override {
    descriptorSets_.push_back(std::move(descriptorSet));
  }

  void BindActorDescriptorSet(const PipelineLayout& pipelineLayout,
                              const CommandBuffer& commandBuffer,
                              const u32 actorIndex) const override {
    commandBuffer.CmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                       pipelineLayout, 1,
                                       descriptorSets_[actorIndex]);
  }

 private:
  std::vector<DescriptorSet> descriptorSets_;
};

class NoResourceBinder : public ResourceBinder {
 public:
  void BindBuffer(const Buffer& buffer, const VkDeviceSize range,
                  const VkDescriptorType descriptorType,
                  const u32 binding) override {}
  void BindTexture(const ImageView& textureView, const u32 binding) override {}
};

class InstantResourceBinder : public ResourceBinder {
 public:
  InstantResourceBinder(const Scene::Initializer& sceneInitializer,
                        const DescriptorSet& descriptorSet)
      : sceneInitializer_(sceneInitializer), descriptorSet_(descriptorSet) {}

  void BindTexture(const ImageView& textureView, const u32 binding) override {
    const VkWriteDescriptorSet writeDescriptorSet =
        sceneInitializer_
            .CreateImageSamplerWrite(descriptorSet_, textureView, binding)
            .Build();
    sceneInitializer_.UpdateDescriptorSets(1, &writeDescriptorSet);
  }

  void BindBuffer(const Buffer& buffer, const VkDeviceSize range,
                  const VkDescriptorType descriptorType,
                  const u32 binding) override {
    const VkWriteDescriptorSet writeDescriptorSet =
        descriptorSet_.CreateBufferWrite(buffer, range, descriptorType, binding)
            .Build();
    sceneInitializer_.UpdateDescriptorSets(1, &writeDescriptorSet);
  }

 private:
  const Scene::Initializer& sceneInitializer_;
  const DescriptorSet& descriptorSet_;
};

class NoResourceBinderFactory : public SceneRenderer::ResourceBinderFactory {
 public:
  std::unique_ptr<ResourceBinder> CreateResourceBinder(
      const DescriptorSet& descriptorSet) const override {
    return std::make_unique<NoResourceBinder>();
  }
};

class InstantResourceBinderFactory
    : public SceneRenderer::ResourceBinderFactory {
 public:
  InstantResourceBinderFactory(const Scene::Initializer& sceneInitializer)
      : sceneInitializer_(sceneInitializer) {}

  std::unique_ptr<ResourceBinder> CreateResourceBinder(
      const DescriptorSet& descriptorSet) const override {
    return std::make_unique<InstantResourceBinder>(sceneInitializer_,
                                                   descriptorSet);
  }

 private:
  const Scene::Initializer& sceneInitializer_;
};

DescriptorPool CreateDescriptorPool(const Scene::Initializer& initializer) {
  constexpr std::array<VkDescriptorPoolSize, 3> descriptorPoolSizes{
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetDescriptorCount(100),
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC)
          .SetDescriptorCount(1),
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetDescriptorCount(100),
  };

  return initializer.CreateDescriptorPool(
      DescriptorPoolCreateInfoBuilder()
          .SetFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
          .SetPoolSizeCount(descriptorPoolSizes.size())
          .SetPPoolSizes(descriptorPoolSizes.data())
          .SetMaxSets(100));
}

Scene::Scene(const Initializer& initializer,
             DynamicUniformBufferInitializer& uniformBufferInitializer,
             const DescriptorSetLayoutFactory& descriptorSetLayoutFactory,
             const RenderPipeline::Initializer& renderPipelineInitializer,
             const ShaderModuleFactory& shaderModuleFactory,
             ResourceLoader& resourceLoader, const wnd::Window& window,
             const u32& imageIndex)
    : renderers_(),
      window_(&window),
      camera_(),
      descriptorPool_(CreateDescriptorPool(initializer)) {
  std::unique_ptr<ParticleSpawner> particleRender =
      std::make_unique<ParticleSpawner>();
  ParticleController& particleController = *particleRender;

  std::vector<std::unique_ptr<SceneRender>> sceneRenders;
  sceneRenders.push_back(std::move(particleRender));
  sceneRenders.push_back(std::make_unique<LightRender>());
  sceneRenders.push_back(std::make_unique<SkyboxRender>());
  sceneRenders.push_back(
      std::make_unique<SpaceshipRender>(camera_, particleController));

  DescriptorSetLayout sceneDescriptorLayout =
      SceneDescriptor::CreateSceneDescriptorLayout(descriptorSetLayoutFactory);

  for (std::unique_ptr<SceneRender>& render : sceneRenders) {
    const std::unique_ptr<PipelineStateFactory> pipelineStateFactory =
        render->ConfigurePipeline();
    const std::unique_ptr<DescriptorConfiguration> descriptorConfiguration =
        render->ConfigureDescriptors();

    std::optional<DescriptorSetLayout> descriptorSetLayout =
        descriptorConfiguration->ConfigureActorDescriptorSet(
            descriptorSetLayoutFactory);

    std::unique_ptr<ActorDescriptorAllocator> descriptorAllocator;
    std::unique_ptr<SceneRenderer::ResourceBinderFactory> resourceBinderFactory;
    std::unique_ptr<ActorDescriptorBinder> descriptorBinder;

    std::vector<const DescriptorSetLayout*> descriptorSetLayouts{
        &sceneDescriptorLayout};

    if (descriptorSetLayout.has_value()) {
      descriptorSetLayouts.push_back(std::move(&descriptorSetLayout.value()));
    }

    RenderPipeline renderPipeline(renderPipelineInitializer,
                                  shaderModuleFactory, descriptorSetLayouts,
                                  *pipelineStateFactory);

    if (descriptorSetLayout.has_value()) {
      descriptorAllocator = std::make_unique<DescriptorAllocator>(
          descriptorPool_, std::move(descriptorSetLayout.value()));
      resourceBinderFactory =
          std::make_unique<InstantResourceBinderFactory>(initializer);
      descriptorBinder = std::make_unique<ArrayDescriptorBinder>();
    } else {
      descriptorAllocator = std::make_unique<NoDescriptorAllocator>();
      resourceBinderFactory = std::make_unique<NoResourceBinderFactory>();
      descriptorBinder = std::make_unique<NoDescriptorBinder>();
    }

    renderers_.push_back(std::move(std::make_unique<SceneRenderer>(
        resourceLoader, std::move(resourceBinderFactory),
        std::move(descriptorAllocator), std::move(render),
        std::move(renderPipeline), std::move(descriptorBinder))));
  }

  sceneDescriptor_ =
      SceneDescriptor(std::move(sceneDescriptorLayout), descriptorPool_,
                      uniformBufferInitializer, imageIndex);
  const VkWriteDescriptorSet writeSceneDescriptorSet =
      sceneDescriptor_.WriteDescriptorSet().Build();
  initializer.UpdateDescriptorSets(1, &writeSceneDescriptorSet);
}

void Scene::UpdateAspect(const float aspect) {
  sceneDescriptor_.UpdateAspect(aspect);
}

void Scene::UpdateModel(const UpdateContext& context) {
  for (std::unique_ptr<SceneRenderer>& renderer : renderers_) {
    renderer->UpdateModel(context);
  }

  PerFrameData& frame = sceneDescriptor_.FrameData();
  frame.view = camera_.GetViewMatrix();
  frame.cameraPosition = camera_.GetPosition();
  frame.lightingPosition = glm::vec3(0.0f, 0.0f, 100000.0f);
  frame.material = {.ambient = glm::vec3(1.0f),
                    .diffuse = glm::vec3(1.0f),
                    .specular = glm::vec3(1.0f),
                    .shininess = 32.0f};
  const glm::vec3 lightColor(77.0f / 255.0f, 77.0f / 255.0f, 1.0f);
  frame.light = {.position = glm::vec3(0.0f, 0.0f, 10000.0f),
                 .ambient = lightColor * 0.02f,
                 .diffuse = lightColor,
                 .specular = lightColor};
}

void Scene::Render(const CommandBuffer& commandBuffer) const {
  sceneDescriptor_.FlushFrameData();

  for (const std::unique_ptr<SceneRenderer>& renderer : renderers_) {
    renderer->Render(commandBuffer, sceneDescriptor_, *window_);
  }
}
