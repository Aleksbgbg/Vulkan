#include "LightRender.h"

#include "LightBox.h"
#include "LightVertex.h"

class LightPipelineStateFactory : public PipelineStateFactory {
 public:
  LightPipelineStateFactory()
      : vertexBindingDescription(
            VertexInputBindingDescriptionBuilder()
                .SetBinding(0)
                .SetStride(sizeof(LightVertex))
                .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX)),
        vertexAttributeDescription(
            VertexInputAttributeDescriptionBuilder()
                .SetBinding(0)
                .SetLocation(0)
                .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                .SetOffset(offsetof(LightVertex, position))) {}

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
        .SetVertexBindingDescriptionCount(1)
        .SetPVertexBindingDescriptions(&vertexBindingDescription)
        .SetVertexAttributeDescriptionCount(1)
        .SetPVertexAttributeDescriptions(&vertexAttributeDescription);
  }

 private:
  const VkVertexInputBindingDescription vertexBindingDescription;
  const VkVertexInputAttributeDescription vertexAttributeDescription;
};

std::unique_ptr<PipelineStateFactory> LightRender::ConfigurePipeline() const {
  return std::make_unique<LightPipelineStateFactory>();
}

std::unique_ptr<DescriptorConfiguration> LightRender::ConfigureDescriptors()
    const {
  return std::make_unique<EmptyDescriptorConfiguration>();
}

std::vector<std::unique_ptr<Actor>> LightRender::LoadActors(
    const ResourceLoader& resourceLoader) const {
  std::vector<std::unique_ptr<Actor>> actors(1);
  actors[0] = std::make_unique<LightBox>(resourceLoader);
  return actors;
}
