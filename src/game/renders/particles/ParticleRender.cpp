#include "ParticleRender.h"

#include <array>
#include <vector>

#include "ParticleGenerator.h"
#include "game/rendering/vertices/PositionVertex.h"

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
  void ConfigureDescriptorPoolSizes(
      std::vector<VkDescriptorPoolSize>& poolSizes) const override {
    poolSizes.push_back(
        std::move(DescriptorPoolSizeBuilder()
                      .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                      .SetDescriptorCount(1)));
  }

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

ParticleRender::ParticleRender()
    : particleGenerator(std::make_unique<ParticleGenerator>()) {}

std::unique_ptr<PipelineStateFactory> ParticleRender::ConfigurePipeline()
    const {
  return std::make_unique<ParticlePipelineStateFactory>();
}

std::unique_ptr<DescriptorConfiguration> ParticleRender::ConfigureDescriptors()
    const {
  return std::make_unique<ParticleDescriptorConfiguration>();
}

std::vector<std::unique_ptr<Actor>> ParticleRender::LoadActors(
    ResourceLoader& resourceLoader) {
  *particleGenerator = std::move(ParticleGenerator(resourceLoader));

  std::vector<std::unique_ptr<Actor>> actors(1);
  actors[0] = std::move(particleGenerator);
  return actors;
}

ParticleController& ParticleRender::GetParticleController() const {
  return *particleGenerator;
}
