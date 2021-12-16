#include "Pipeline.h"

#include <algorithm>

#include "error.h"

Pipeline::Pipeline() : pipeline(nullptr) {}

Pipeline::Pipeline(VkDevice device, const PipelineCache& pipelineCache,
                   const std::vector<ShaderModule>& shaderModules,
                   PipelineLayout pipelineLayout,
                   const SubpassReference subpassReference,
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

Pipeline::Pipeline(Pipeline&& other) noexcept
    : device(other.device),
      pipeline(other.pipeline),
      pipelineLayout(std::move(other.pipelineLayout)) {
  other.pipeline = nullptr;
}

Pipeline::~Pipeline() {
  if (pipeline != nullptr) {
    vkDestroyPipeline(device, pipeline, nullptr);
  }
}

Pipeline& Pipeline::operator=(Pipeline&& other) noexcept {
  std::swap(device, other.device);
  std::swap(pipeline, other.pipeline);
  pipelineLayout = std::move(other.pipelineLayout);
  return *this;
}

const PipelineLayout& Pipeline::GetLayout() const {
  return pipelineLayout;
}