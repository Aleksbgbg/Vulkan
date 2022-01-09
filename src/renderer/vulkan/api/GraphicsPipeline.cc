#include "GraphicsPipeline.h"

#include <algorithm>

#include "error.h"

namespace vk {

GraphicsPipeline::GraphicsPipeline() : pipeline(nullptr) {}

GraphicsPipeline::GraphicsPipeline(
    VkDevice device, const PipelineCache& pipelineCache,
    const std::vector<ShaderModule>& shaderModules,
    PipelineLayout pipelineLayout, const SubpassReference subpassReference,
    GraphicsPipelineCreateInfoBuilder infoBuilder)
    : device(device), pipelineLayout(std::move(pipelineLayout)) {
  std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos(
      shaderModules.size());
  std::transform(shaderModules.begin(), shaderModules.end(),
                 shaderCreateInfos.begin(),
                 [](const ShaderModule& shaderModule) {
                   return PipelineShaderStageCreateInfoBuilder()
                       .SetStage(shaderModule.stage)
                       .SetModule(shaderModule.shaderModule)
                       .SetPName("main");
                 });
  PROCEED_ON_VALID_RESULT(vkCreateGraphicsPipelines(
      device, pipelineCache.pipelineCache, 1,
      infoBuilder.SetStageCount(shaderCreateInfos.size())
          .SetPStages(shaderCreateInfos.data())
          .SetLayout(this->pipelineLayout.pipelineLayout)
          .SetRenderPass(subpassReference.renderPass->renderPass)
          .SetSubpass(subpassReference.subpass)
          .Build(),
      nullptr, &pipeline));
}

GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& other) noexcept
    : device(other.device),
      pipeline(other.pipeline),
      pipelineLayout(std::move(other.pipelineLayout)) {
  other.pipeline = nullptr;
}

GraphicsPipeline::~GraphicsPipeline() {
  if (pipeline != nullptr) {
    vkDestroyPipeline(device, pipeline, nullptr);
  }
}

GraphicsPipeline& GraphicsPipeline::operator=(
    GraphicsPipeline&& other) noexcept {
  this->~GraphicsPipeline();
  new (this) GraphicsPipeline(std::move(other));
  return *this;
}

const PipelineLayout& GraphicsPipeline::GetLayout() const {
  return pipelineLayout;
}

}  // namespace vk
