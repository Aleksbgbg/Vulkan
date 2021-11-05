#include "Scene.h"

#include <algorithm>

#include "game/renders/light/LightRender.h"
#include "game/renders/sky/SkyboxRender.h"
#include "game/renders/spaceships/SpaceshipRender.h"

class NoDescriptorBinder : public ActorDescriptorBinder {
 public:
  void BindActorDescriptorSet(const PipelineLayout& pipelineLayout,
                              const CommandBuffer& commandBuffer,
                              const u32 actorIndex) const override {}
};

class ArrayDescriptorBinder : public ActorDescriptorBinder {
 public:
  ArrayDescriptorBinder(std::vector<DescriptorSet> descriptorSets)
      : descriptorSets(std::move(descriptorSets)) {}

  void BindActorDescriptorSet(const PipelineLayout& pipelineLayout,
                              const CommandBuffer& commandBuffer,
                              const u32 actorIndex) const override {
    commandBuffer.CmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                       pipelineLayout, 1,
                                       descriptorSets[actorIndex]);
  }

 private:
  std::vector<DescriptorSet> descriptorSets;
};

static constexpr const u32 RenderCount = 3;

Scene::Scene(const VulkanContext& vulkanContext,
             const ResourceLoader& resourceLoader, const Window& window,
             const u32& imageIndex,
             const DynamicUniformBufferInitializer& uniformBufferInitializer)
    : renderers(RenderCount), window(&window), camera() {
  std::vector<std::unique_ptr<SceneRender>> sceneRenders(RenderCount);
  sceneRenders[0] = std::make_unique<LightRender>();
  sceneRenders[1] = std::make_unique<SkyboxRender>();
  sceneRenders[2] = std::make_unique<SpaceshipRender>(camera, window);

  std::vector<VkDescriptorPoolSize> descriptorPoolSizes;
  SceneDescriptor::ConfigureDescriptorPoolSizes(descriptorPoolSizes);
  u32 descriptorSetCount = 1;

  DescriptorSetLayout sceneDescriptorLayout =
      SceneDescriptor::CreateLayout(*vulkanContext.virtualDevice);

  struct RenderInit {
    RenderPipeline renderPipeline;
    std::vector<std::unique_ptr<Actor>> actors;
    std::optional<DescriptorSetLayout> actorDescriptorSetLayout;
  };

  std::vector<RenderInit> renderInits(RenderCount);

  for (u32 renderIndex = 0; renderIndex < sceneRenders.size(); ++renderIndex) {
    RenderInit& renderInit = renderInits[renderIndex];
    const std::unique_ptr<SceneRender>& render = sceneRenders[renderIndex];

    const std::unique_ptr<PipelineStateFactory> pipelineStateFactory =
        render->ConfigurePipeline();
    const std::unique_ptr<DescriptorConfiguration> descriptorConfiguration =
        render->ConfigureDescriptors();
    renderInit.actors = render->LoadActors(resourceLoader);

    descriptorConfiguration->ConfigureDescriptorPoolSizes(descriptorPoolSizes);
    std::optional<DescriptorSetLayout> actorDescriptorSetLayout =
        descriptorConfiguration->ConfigureActorDescriptorSet(
            *vulkanContext.virtualDevice);

    std::vector<const DescriptorSetLayout*> descriptorSetLayouts = {
        &sceneDescriptorLayout};

    if (actorDescriptorSetLayout.has_value()) {
      descriptorSetLayouts.push_back(&actorDescriptorSetLayout.value());
      descriptorSetCount += renderInit.actors.size();
    }

    renderInit.renderPipeline = RenderPipeline(
        *vulkanContext.virtualDevice, *vulkanContext.pipelineCache,
        std::move(descriptorSetLayouts), *vulkanContext.subpassReference,
        vulkanContext.samples, *pipelineStateFactory);
    renderInit.actorDescriptorSetLayout = std::move(actorDescriptorSetLayout);
  }

  descriptorPool = vulkanContext.virtualDevice->CreateDescriptorPool(
      DescriptorPoolCreateInfoBuilder()
          .SetPPoolSizes(descriptorPoolSizes.data())
          .SetPoolSizeCount(descriptorPoolSizes.size())
          .SetMaxSets(descriptorSetCount));

  std::vector<DescriptorSet::WriteDescriptorSet> descriptorSetWrites;

  sceneDescriptor =
      SceneDescriptor(*vulkanContext.virtualDevice, descriptorPool,
                      std::move(sceneDescriptorLayout), resourceLoader,
                      imageIndex, uniformBufferInitializer);
  sceneDescriptor.WriteDescriptorSets(descriptorSetWrites);

  for (u32 renderIndex = 0; renderIndex < sceneRenders.size(); ++renderIndex) {
    RenderInit& renderInit = renderInits[renderIndex];

    std::unique_ptr<ActorDescriptorBinder> descriptorBinder;

    if (renderInit.actorDescriptorSetLayout.has_value()) {
      std::vector<DescriptorSet> descriptorSets(
          descriptorPool.AllocateDescriptorSets(
              renderInit.actorDescriptorSetLayout.value(),
              renderInit.actors.size()));

      for (u32 actorIndex = 0; actorIndex < renderInit.actors.size();
           ++actorIndex) {
        const std::unique_ptr<Actor>& actor = renderInit.actors[actorIndex];
        const DescriptorSet& descriptorSet = descriptorSets[actorIndex];

        TextureRegistry textureRegistry(descriptorSet, *vulkanContext.sampler,
                                        descriptorSetWrites);
        actor->WriteTexture(textureRegistry);
      }

      descriptorBinder =
          std::make_unique<ArrayDescriptorBinder>(std::move(descriptorSets));

    } else {
      descriptorBinder = std::make_unique<NoDescriptorBinder>();
    }

    renderers[renderIndex] = std::move(SceneRenderer(
        std::move(renderInit.renderPipeline), std::move(descriptorBinder),
        std::move(renderInit.actors)));
  }

  std::vector<VkWriteDescriptorSet> writeDescriptorSets(
      descriptorSetWrites.size());
  std::transform(
      descriptorSetWrites.begin(), descriptorSetWrites.end(),
      writeDescriptorSets.begin(),
      [](const DescriptorSet::WriteDescriptorSet& writeDescriptorSet) {
        return writeDescriptorSet.Build();
      });
  vulkanContext.virtualDevice->UpdateDescriptorSets(writeDescriptorSets.size(),
                                                    writeDescriptorSets.data());
}

const Camera& Scene::GetCamera() const {
  return camera;
}

void Scene::UpdateAspect(const float aspect) {
  sceneDescriptor.UpdateAspect(aspect);
}

void Scene::UpdateModel(const UpdateContext& context) {
  for (SceneRenderer& renderer : renderers) {
    renderer.UpdateModel(context);
  }

  const Camera::View cameraView = camera.GetView();

  PerFrameData& frame = sceneDescriptor.FrameData();
  frame.view = glm::lookAt(cameraView.position, cameraView.lookAt,
                           glm::vec3(0.0f, 1.0f, 0.0f));
  frame.cameraPosition = cameraView.position;
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
  sceneDescriptor.FlushFrameData();

  for (const SceneRenderer& renderer : renderers) {
    renderer.Render(commandBuffer, sceneDescriptor, *window);
  }
}
