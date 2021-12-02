#include "LightRender.h"

#include <array>

#include "LightBox.h"
#include "game/rendering/vertices/PositionTextureVertex.h"

class LightPipelineStateFactory : public PipelineStateFactory {
 public:
  LightPipelineStateFactory()
      : pushConstantsRange(PushConstantRangeBuilder()
                               .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT)
                               .SetOffset(0)
                               .SetSize(sizeof(glm::mat4))),
        vertexBindingDescriptions({
            VertexInputBindingDescriptionBuilder()
                .SetBinding(0)
                .SetStride(sizeof(PositionTextureVertex))
                .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
        }),
        vertexAttributeDescriptions({
            VertexInputAttributeDescriptionBuilder()
                .SetBinding(0)
                .SetLocation(0)
                .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                .SetOffset(offsetof(PositionTextureVertex, position)),
            VertexInputAttributeDescriptionBuilder()
                .SetBinding(0)
                .SetLocation(1)
                .SetFormat(VK_FORMAT_R32G32_SFLOAT)
                .SetOffset(offsetof(PositionTextureVertex, textureCoordinate)),
        }) {}

  std::vector<ShaderModule> LoadShaders(
      const ShaderModuleFactory& shaderModuleFactory) const override {
    std::vector<ShaderModule> shaders;
    shaders.emplace_back(shaderModuleFactory.LoadShader(
        VK_SHADER_STAGE_VERTEX_BIT, "shaders/light.vert.spv"));
    shaders.emplace_back(shaderModuleFactory.LoadShader(
        VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/light.frag.spv"));
    return shaders;
  }

  PipelineLayoutCreateInfoBuilder CreatePipelineLayout() const override {
    return PipelineLayoutCreateInfoBuilder()
        .SetPushConstantRangeCount(1)
        .SetPPushConstantRanges(&pushConstantsRange);
  }

  PipelineVertexInputStateCreateInfoBuilder CreateVertexInputState()
      const override {
    return PipelineVertexInputStateCreateInfoBuilder()
        .SetVertexBindingDescriptionCount(vertexBindingDescriptions.size())
        .SetPVertexBindingDescriptions(vertexBindingDescriptions.data())
        .SetVertexAttributeDescriptionCount(vertexAttributeDescriptions.size())
        .SetPVertexAttributeDescriptions(vertexAttributeDescriptions.data());
  }

 private:
  const VkPushConstantRange pushConstantsRange;
  const std::array<VkVertexInputBindingDescription, 1>
      vertexBindingDescriptions;
  const std::array<VkVertexInputAttributeDescription, 2>
      vertexAttributeDescriptions;
};

class LightboxDescriptorConfiguration : public DescriptorConfiguration {
 public:
  std::optional<DescriptorSetLayout> ConfigureActorDescriptorSet(
      const DescriptorSetLayoutFactory& descriptorSetLayoutFactory)
      const override {
    constexpr const VkDescriptorSetLayoutBinding skyboxTextureSampler =
        DescriptorSetLayoutBindingBuilder()
            .SetBinding(0)
            .SetDescriptorCount(1)
            .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT);
    return descriptorSetLayoutFactory.CreateDescriptorSetLayout(
        DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
            &skyboxTextureSampler));
  }
};

std::unique_ptr<PipelineStateFactory> LightRender::ConfigurePipeline() const {
  return std::make_unique<LightPipelineStateFactory>();
}

std::unique_ptr<DescriptorConfiguration> LightRender::ConfigureDescriptors()
    const {
  return std::make_unique<LightboxDescriptorConfiguration>();
}

void LightRender::LoadActors(ResourceLoader& resourceLoader,
                             ActorSpawnController& actorSpawnController) {
  std::vector<std::unique_ptr<Actor>> actors(1);
  actors[0] = std::make_unique<LightBox>(resourceLoader);
  actorSpawnController.SpawnActorsImmediately(std::move(actors));
}
