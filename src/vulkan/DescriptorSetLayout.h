#ifndef VULKAN_SRC_VULKAN_DESCRIPTORSETLAYOUT_H
#define VULKAN_SRC_VULKAN_DESCRIPTORSETLAYOUT_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/DescriptorSetLayoutCreateInfo.h"

class DescriptorSetLayout {
  friend class PipelineLayout;
  friend class DescriptorPool;

 public:
  VULKAN_OBJECT_MOVABLE_ROOT(DescriptorSetLayout, device, descriptorSetLayout)

  DescriptorSetLayout() = default;
  DescriptorSetLayout(VkDevice device,
                      DescriptorSetLayoutCreateInfoBuilder& infoBuilder);

  DescriptorSetLayout(const DescriptorSetLayout&) = delete;

  ~DescriptorSetLayout();

  DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

 private:
  VkDevice device;
  VkDescriptorSetLayout descriptorSetLayout = nullptr;
};

#endif  // VULKAN_SRC_VULKAN_DESCRIPTORSETLAYOUT_H
