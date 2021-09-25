#include <algorithm>
#include "Pipeline.h"

#include "error.h"

Pipeline::Pipeline(VkDevice device, const std::vector<ShaderModule>& shaderModules, PipelineLayout pipelineLayout,
                   RenderPass renderPass, GraphicsPipelineCreateInfoBuilder& infoBuilder)
  :
    device(device),
    pipelineLayout(std::move(pipelineLayout)),
    renderPass(std::move(renderPass)) {
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
              .SetRenderPass(this->renderPass.renderPass)
              .Build(),
          nullptr,
          &pipeline))
}

Pipeline::~Pipeline() {
  if (pipeline != nullptr) {
    vkDestroyPipeline(device, pipeline, nullptr);
  }
}

RenderPass& Pipeline::GetRenderPass() {
  return renderPass;
}

PipelineLayout& Pipeline::GetLayout() {
  return pipelineLayout;
}
