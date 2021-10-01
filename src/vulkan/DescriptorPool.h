#ifndef VULKAN_SRC_VULKAN_DESCRIPTORPOOL_H
#define VULKAN_SRC_VULKAN_DESCRIPTORPOOL_H

#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/structures/DescriptorSetAllocateInfo.h>

#include "lifetime_semantics.h"
#include "structures/DescriptorPoolCreateInfo.h"
#include "DescriptorSet.h"
#include "DescriptorSetLayout.h"
#include "Pipeline.h"

class DescriptorPool {
  friend class ImGuiInstance;
public:
  VULKAN_OBJECT_MOVABLE_ROOT(DescriptorPool, device, descriptorPool)

  DescriptorPool() = default;
  DescriptorPool(VkDevice device, DescriptorPoolCreateInfoBuilder& infoBuilder);

  DescriptorPool(const DescriptorPool&) = delete;

  ~DescriptorPool();

  DescriptorPool& operator=(const DescriptorPool&) = delete;

  std::vector<DescriptorSet> AllocateDescriptorSets(DescriptorSetLayout& descriptorSetLayout, const u32 count) const;

private:
  VkDevice device;
  VkDescriptorPool descriptorPool = nullptr;
};

#endif // VULKAN_SRC_VULKAN_DESCRIPTORPOOL_H
