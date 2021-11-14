#ifndef VULKAN_SRC_VULKAN_DESCRIPTORSETLAYOUT_H
#define VULKAN_SRC_VULKAN_DESCRIPTORSETLAYOUT_H

#include <vulkan/vulkan.h>

#include "structures/DescriptorSetLayoutCreateInfo.h"

class DescriptorSetLayout {
  friend class PipelineLayout;
  friend class DescriptorPool;

 public:
  DescriptorSetLayout();
  DescriptorSetLayout(VkDevice device,
                      const DescriptorSetLayoutCreateInfoBuilder& infoBuilder);

  DescriptorSetLayout(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout(DescriptorSetLayout&& other) noexcept;

  ~DescriptorSetLayout();

  DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
  DescriptorSetLayout& operator=(DescriptorSetLayout&& other) noexcept;

 private:
  VkDevice device;
  VkDescriptorSetLayout descriptorSetLayout;
};

#endif  // VULKAN_SRC_VULKAN_DESCRIPTORSETLAYOUT_H
