#ifndef VULKAN_SRC_VULKAN_PIPELINELAYOUT_H_
#define VULKAN_SRC_VULKAN_PIPELINELAYOUT_H_

#include <vulkan/vulkan.h>

#include <vector>

#include "DescriptorSetLayout.h"
#include "structures/PipelineLayoutCreateInfo.h"

class PipelineLayout {
  friend class Pipeline;
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

#endif  // VULKAN_SRC_VULKAN_PIPELINELAYOUT_H_
