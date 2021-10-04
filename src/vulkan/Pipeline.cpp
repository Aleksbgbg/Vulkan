#include "Pipeline.h"

#include <algorithm>

#include "error.h"

Pipeline::Pipeline(
    VkDevice device,
    const std::vector<ShaderModule>& shaderModules,
    PipelineLayout pipelineLayout,
    const SubpassReference subpassReference,
    GraphicsPipelineCreateInfoBuilder& infoBuilder)
  :
    device(device),
    pipelineLayout(std::move(pipelineLayout)) {
  std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos(shaderModules.size());
  std::transform(
      shaderModules.begin(),
      shaderModules.end(),
      shaderCreateInfos.begin(),
      [](const ShaderModule& shaderModule) {
        return PipelineShaderStageCreateInfoBuilder()
                   .SetStage(shaderModule.stage)
                   .SetModule(shaderModule.shaderModule)
                   .SetPName("main")
                   .BuildObject();
      });
  PROCEED_ON_VALID_RESULT(
      vkCreateGraphicsPipelines(
          device,
          VK_NULL_HANDLE,
          1,
          infoBuilder
              .SetStageCount(shaderCreateInfos.size())
              .SetPStages(shaderCreateInfos.data())
              .SetLayout(this->pipelineLayout.pipelineLayout)
              .SetRenderPass(subpassReference.renderPass.renderPass)
              .SetSubpass(subpassReference.subpass)
              .Build(),
          nullptr,
          &pipeline))
}

Pipeline::~Pipeline() {
  if (pipeline != nullptr) {
    vkDestroyPipeline(device, pipeline, nullptr);
  }
}

PipelineLayout& Pipeline::GetLayout() {
  return pipelineLayout;
}
