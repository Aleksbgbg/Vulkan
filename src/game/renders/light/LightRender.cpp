#include "LightRender.h"

#include <array>

#include "LightBox.h"
#include "game/rendering/vertices/PositionTextureVertex.h"

class LightPipelineStateFactory : public PipelineStateFactory {
 public:
  LightPipelineStateFactory()
      : vertexBindingDescriptions({
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
      const VirtualDevice& virtualDevice) const override {
    std::vector<ShaderModule> shaders;
    shaders.emplace_back(virtualDevice.LoadShader(VK_SHADER_STAGE_VERTEX_BIT,
                                                  "shaders/light.vert.spv"));
    shaders.emplace_back(virtualDevice.LoadShader(VK_SHADER_STAGE_FRAGMENT_BIT,
                                                  "shaders/light.frag.spv"));
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
        .SetVertexAttributeDescriptionCount(vertexAttributeDescriptions.size())
        .SetPVertexAttributeDescriptions(vertexAttributeDescriptions.data());
  }

 private:
  const std::array<VkVertexInputBindingDescription, 1>
      vertexBindingDescriptions;
  const std::array<VkVertexInputAttributeDescription, 2>
      vertexAttributeDescriptions;
};

class LightboxDescriptorConfiguration : public DescriptorConfiguration {
 public:
  void ConfigureDescriptorPoolSizes(
      std::vector<VkDescriptorPoolSize>& poolSizes) const override {
    poolSizes.push_back(DescriptorPoolSizeBuilder()
                            .SetType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
                            .SetDescriptorCount(1));
  }

  std::optional<DescriptorSetLayout> ConfigureActorDescriptorSet(
      const VirtualDevice& virtualDevice) const override {
    constexpr const VkDescriptorSetLayoutBinding skyboxTextureSampler =
        DescriptorSetLayoutBindingBuilder()
            .SetBinding(0)
            .SetDescriptorCount(1)
            .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT);
    return virtualDevice.CreateDescriptorSetLayout(
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

std::vector<std::unique_ptr<Actor>> LightRender::LoadActors(
    const ResourceLoader& resourceLoader) const {
  std::vector<std::unique_ptr<Actor>> actors(1);
  actors[0] = std::make_unique<LightBox>(resourceLoader);
  return actors;
}