#include "ComputePipeline.h"

#include <array>

#include "error.h"
#include "renderer/vulkan/api/structures/ComputePipelineCreateInfo.h"
#include "renderer/vulkan/api/structures/SpecializationInfo.h"
#include "renderer/vulkan/api/structures/SpecializationMapEntry.h"

namespace vk {

ComputePipeline::ComputePipeline() : pipeline_(nullptr) {}

ComputePipeline::ComputePipeline(VkDevice device,
                                 const PipelineCache& pipelineCache,
                                 PipelineLayout pipelineLayout,
                                 const ShaderModule& shaderModule)
    : device_(device), pipelineLayout_(std::move(pipelineLayout)) {
  constexpr std::array<VkSpecializationMapEntry, 3> specializationEntries{
      SpecializationMapEntryBuilder().SetConstantID(0).SetOffset(0).SetSize(
          sizeof(i32)),
      SpecializationMapEntryBuilder()
          .SetConstantID(1)
          .SetOffset(sizeof(i32) * 1)
          .SetSize(sizeof(i32)),
      SpecializationMapEntryBuilder()
          .SetConstantID(2)
          .SetOffset(sizeof(i32) * 2)
          .SetSize(sizeof(i32)),
  };
  constexpr i32 constants[3] = {64, 4, 1};
  PROCEED_ON_VALID_RESULT(vkCreateComputePipelines(
      device_, pipelineCache.pipelineCache, 1,
      ComputePipelineCreateInfoBuilder()
          .SetStage(PipelineShaderStageCreateInfoBuilder()
                        .SetPSpecializationInfo(
                            SpecializationInfoBuilder()
                                .SetMapEntryCount(specializationEntries.size())
                                .SetPMapEntries(specializationEntries.data())
                                .SetDataSize(sizeof(constants))
                                .SetPData(constants)
                                .Build())
                        .SetStage(shaderModule.stage)
                        .SetModule(shaderModule.shaderModule)
                        .SetPName("main"))
          .SetLayout(pipelineLayout_.pipelineLayout)
          .Build(),
      nullptr, &pipeline_));
}

ComputePipeline::ComputePipeline(ComputePipeline&& other) noexcept
    : device_(other.device_),
      pipeline_(other.pipeline_),
      pipelineLayout_(std::move(other.pipelineLayout_)) {
  other.pipeline_ = nullptr;
}

ComputePipeline::~ComputePipeline() {
  if (pipeline_ != nullptr) {
    vkDestroyPipeline(device_, pipeline_, nullptr);
  }
}

ComputePipeline& ComputePipeline::operator=(ComputePipeline&& other) noexcept {
  std::swap(device_, other.device_);
  std::swap(pipeline_, other.pipeline_);
  pipelineLayout_ = std::move(other.pipelineLayout_);
  return *this;
}

const PipelineLayout& ComputePipeline::GetLayout() const {
  return pipelineLayout_;
}

}  // namespace vk
