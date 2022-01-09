#ifndef VULKAN_SRC_RENDERER_VULKAN_API_PIPELINELAYOUT_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_PIPELINELAYOUT_H_

#include <vulkan/vulkan.h>

#include <vector>

#include "DescriptorSetLayout.h"
#include "renderer/vulkan/api/structures/PipelineLayoutCreateInfo.h"

namespace vk {

class PipelineLayout {
  friend class GraphicsPipeline;
  friend class ComputePipeline;
  friend class CommandBuffer;

 public:
  PipelineLayout();
  PipelineLayout(
      VkDevice device,
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts);
  PipelineLayout(
      VkDevice device, PipelineLayoutCreateInfoBuilder infoBuilder,
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts);

  PipelineLayout(const PipelineLayout&) = delete;
  PipelineLayout(PipelineLayout&& other) noexcept;

  ~PipelineLayout();

  PipelineLayout& operator=(const PipelineLayout&) = delete;
  PipelineLayout& operator=(PipelineLayout&& other) noexcept;

 private:
  VkDevice device;
  VkPipelineLayout pipelineLayout;
};

}  // namespace vk

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_PIPELINELAYOUT_H_
