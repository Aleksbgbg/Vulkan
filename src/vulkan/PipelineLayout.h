#ifndef VULKAN_SRC_VULKAN_PIPELINELAYOUT_H
#define VULKAN_SRC_VULKAN_PIPELINELAYOUT_H

#include <vulkan/vulkan.h>

#include <vector>

#include "DescriptorSetLayout.h"
#include "lifetime_semantics.h"
#include "structures/PipelineLayoutCreateInfo.h"

class PipelineLayout {
  friend class Pipeline;
  friend class CommandBuffer;

 public:
  PipelineLayout() = default;
  PipelineLayout(
      VkDevice device,
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts);
  PipelineLayout(
      VkDevice device, PipelineLayoutCreateInfoBuilder& infoBuilder,
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts);

  PipelineLayout(const PipelineLayout&) = delete;
  PipelineLayout(PipelineLayout&& other) noexcept
      : device(other.device), pipelineLayout(other.pipelineLayout) {
    other.pipelineLayout = nullptr;
  }

  ~PipelineLayout();

  PipelineLayout& operator=(const PipelineLayout&) = delete;
  PipelineLayout& operator=(PipelineLayout&& other) noexcept {
    device = other.device;
    std::swap(pipelineLayout, other.pipelineLayout);
    return *this;
  }

 private:
  VkDevice device;
  VkPipelineLayout pipelineLayout = nullptr;
};

#endif  // VULKAN_SRC_VULKAN_PIPELINELAYOUT_H
