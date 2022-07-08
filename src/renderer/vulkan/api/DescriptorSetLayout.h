#ifndef SRC_RENDERER_VULKAN_API_DESCRIPTORSETLAYOUT_H_
#define SRC_RENDERER_VULKAN_API_DESCRIPTORSETLAYOUT_H_

#include <vulkan/vulkan.h>

#include "renderer/vulkan/api/structures/DescriptorSetLayoutCreateInfo.h"

namespace vk {

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

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_DESCRIPTORSETLAYOUT_H_
