#ifndef VULKAN_SRC_RENDERER_VULKAN_API_COMPUTEPIPELINE_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_COMPUTEPIPELINE_H_

#include <vulkan/vulkan.h>

#include "PipelineCache.h"
#include "PipelineLayout.h"
#include "ShaderModule.h"

class ComputePipeline {
  friend class CommandBuffer;

 public:
  ComputePipeline();
  ComputePipeline(VkDevice device, const PipelineCache& pipelineCache,
                  PipelineLayout pipelineLayout,
                  const ShaderModule& shaderModule);

  ComputePipeline(const ComputePipeline&) = delete;
  ComputePipeline(ComputePipeline&& other) noexcept;

  ~ComputePipeline();

  ComputePipeline& operator=(const ComputePipeline&) = delete;
  ComputePipeline& operator=(ComputePipeline&& other) noexcept;

  const PipelineLayout& GetLayout() const;

 private:
  VkDevice device_;
  VkPipeline pipeline_;
  PipelineLayout pipelineLayout_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_COMPUTEPIPELINE_H_
