#include "ParticleSpawner.h"

#include <array>
#include <vector>

#include "ParticleInstance.h"
#include "game/rendering/vertices/PositionVertex.h"
#include "game/renders/ActorSpawnController.h"

class ParticlePipelineStateFactory : public PipelineStateFactory {
 public:
  ParticlePipelineStateFactory()
      : vertexBindingDescriptions({
            VertexInputBindingDescriptionBuilder()
                .SetBinding(0)
                .SetStride(sizeof(PositionVertex))
                .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
        }),
        vertexAttributeDescription(
            VertexInputAttributeDescriptionBuilder()
                .SetBinding(0)
                .SetLocation(0)
                .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                .SetOffset(offsetof(PositionVertex, position))) {}

  std::vector<ShaderModule> LoadShaders(
      const ShaderModuleFactory& shaderModuleFactory) const override {
    std::vector<ShaderModule> shaders;
    shaders.emplace_back(shaderModuleFactory.LoadShader(
        VK_SHADER_STAGE_VERTEX_BIT, "shaders/particles.vert.spv"));
    shaders.emplace_back(shaderModuleFactory.LoadShader(
        VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/particles.frag.spv"));
    return shaders;
  }

  PipelineLayoutCreateInfoBuilder CreatePipelineLayout() const override {
    return PipelineLayoutCreateInfoBuilder();
  }

  PipelineVertexInputStateCreateInfoBuilder CreateVertexInputState()
      const override {
    return PipelineVertexInputStateCreateInfoBuilder()
        .SetVertexBindingDescriptionCount(vertexBindingDescriptions.size())
        .SetPVertexBindingDescriptions(vertexBindingDescriptions.data())
        .SetVertexAttributeDescriptionCount(1)
        .SetPVertexAttributeDescriptions(&vertexAttributeDescription);
  }

 private:
  const std::array<VkVertexInputBindingDescription, 1>
      vertexBindingDescriptions;
  const VkVertexInputAttributeDescription vertexAttributeDescription;
};

class ParticleDescriptorConfiguration : public DescriptorConfiguration {
 public:
  std::optional<DescriptorSetLayout> ConfigureActorDescriptorSet(
      const DescriptorSetLayoutFactory& descriptorSetLayoutFactory)
      const override {
    constexpr const VkDescriptorSetLayoutBinding particleDescriptorSet =
        DescriptorSetLayoutBindingBuilder()
            .SetBinding(0)
            .SetDescriptorCount(1)
            .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
            .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT);
    return descriptorSetLayoutFactory.CreateDescriptorSetLayout(
        DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
            &particleDescriptorSet));
  }
};

MultiParticleMesh LoadMesh(ResourceLoader& resourceLoader) {
  constexpr float scale = 0.05f;
  const std::vector<PositionVertex> vertices = {
      /* 0 */ {{-scale, -scale, -scale}},
      /* 1 */ {{-scale, scale, -scale}},
      /* 2 */ {{scale, scale, -scale}},
      /* 3 */ {{scale, -scale, -scale}},
      /* 4 */ {{0.0f, 0.0f, scale}},
  };
  const std::vector<u16> indices = {
      0, 1, 2, 2, 3, 0,  // bottom face
      0, 4, 1,           // back face
      1, 4, 2,           // left face
      2, 4, 3,           // right face
      3, 4, 0,           // front face
  };

  return MultiParticleMesh(resourceLoader.AllocateVertexBuffer(vertices),
                           resourceLoader.AllocateIndexBuffer(indices),
                           indices.size());
}

std::unique_ptr<PipelineStateFactory> ParticleSpawner::ConfigurePipeline()
    const {
  return std::make_unique<ParticlePipelineStateFactory>();
}

std::unique_ptr<DescriptorConfiguration> ParticleSpawner::ConfigureDescriptors()
    const {
  return std::make_unique<ParticleDescriptorConfiguration>();
}

void ParticleSpawner::LoadActors(ResourceLoader& resourceLoader,
                                 ActorSpawnController& actorSpawnController) {
  actorSpawnController_ = &actorSpawnController;
  mesh_ = LoadMesh(resourceLoader);
}

ParticleStream& ParticleSpawner::CreateParticleStream() {
  return *reinterpret_cast<ParticleInstance*>(
      &actorSpawnController_->SpawnActor(*this));
}

std::unique_ptr<Actor> ParticleSpawner::CreateActor(
    ResourceLoader& resourceLoader, ResourceBinder& resourceBinder) const {
  return std::make_unique<ParticleInstance>(mesh_, resourceLoader,
                                            resourceBinder);
}
