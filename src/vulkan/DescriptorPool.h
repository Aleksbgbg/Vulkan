#ifndef VULKAN_SRC_VULKAN_DESCRIPTORPOOL_H_
#define VULKAN_SRC_VULKAN_DESCRIPTORPOOL_H_

#include <vulkan/vulkan.h>

#include <vector>

#include "DescriptorSet.h"
#include "DescriptorSetLayout.h"
#include "Pipeline.h"
#include "structures/DescriptorPoolCreateInfo.h"

class DescriptorPool {
  friend class ImGuiInstance;

 public:
  DescriptorPool();
  DescriptorPool(VkDevice device,
                 const DescriptorPoolCreateInfoBuilder& infoBuilder);

  DescriptorPool(const DescriptorPool&) = delete;
  DescriptorPool(DescriptorPool&& other) noexcept;

  ~DescriptorPool();

  DescriptorPool& operator=(const DescriptorPool&) = delete;
  DescriptorPool& operator=(DescriptorPool&& other) noexcept;

  DescriptorSet AllocateDescriptorSet(
      const DescriptorSetLayout& descriptorSetLayout) const;
  std::vector<DescriptorSet> AllocateDescriptorSets(
      const DescriptorSetLayout& descriptorSetLayout, const u32 count) const;

 private:
  VkDevice device;
  VkDescriptorPool descriptorPool;
};

#endif  // VULKAN_SRC_VULKAN_DESCRIPTORPOOL_H_
