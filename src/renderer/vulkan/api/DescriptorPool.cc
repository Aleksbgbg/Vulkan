#include "DescriptorPool.h"

#include <algorithm>
#include <iterator>

#include "error.h"
#include "renderer/vulkan/api/structures/DescriptorSetAllocateInfo.h"

namespace vk {

DescriptorPool::DescriptorPool() : descriptorPool(nullptr) {}

DescriptorPool::DescriptorPool(
    VkDevice device, const DescriptorPoolCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(vkCreateDescriptorPool(device, infoBuilder.Build(),
                                                 nullptr, &descriptorPool));
}

DescriptorPool::DescriptorPool(DescriptorPool&& other) noexcept
    : device(other.device), descriptorPool(other.descriptorPool) {
  other.descriptorPool = nullptr;
}

DescriptorPool::~DescriptorPool() {
  if (descriptorPool != nullptr) {
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
  }
}

DescriptorPool& DescriptorPool::operator=(DescriptorPool&& other) noexcept {
  this->~DescriptorPool();
  new (this) DescriptorPool(std::move(other));
  return *this;
}

DescriptorSet DescriptorPool::AllocateDescriptorSet(
    const DescriptorSetLayout& descriptorSetLayout) const {
  VkDescriptorSet descriptorSet;
  PROCEED_ON_VALID_RESULT(vkAllocateDescriptorSets(
      device,
      DescriptorSetAllocateInfoBuilder()
          .SetDescriptorPool(descriptorPool)
          .SetDescriptorSetCount(1)
          .SetPSetLayouts(&descriptorSetLayout.descriptorSetLayout)
          .Build(),
      &descriptorSet));
  return DescriptorSet(descriptorSet);
}

std::vector<DescriptorSet> DescriptorPool::AllocateDescriptorSets(
    const DescriptorSetLayout& descriptorSetLayout, const u32 count) const {
  std::vector<VkDescriptorSetLayout> rawLayouts(
      count, descriptorSetLayout.descriptorSetLayout);
  std::vector<VkDescriptorSet> rawDescriptorSets(count);
  PROCEED_ON_VALID_RESULT(
      vkAllocateDescriptorSets(device,
                               DescriptorSetAllocateInfoBuilder()
                                   .SetDescriptorPool(descriptorPool)
                                   .SetDescriptorSetCount(count)
                                   .SetPSetLayouts(rawLayouts.data())
                                   .Build(),
                               rawDescriptorSets.data()));
  std::vector<DescriptorSet> descriptorSets(count);
  std::transform(rawDescriptorSets.begin(), rawDescriptorSets.end(),
                 descriptorSets.begin(), [](VkDescriptorSet descriptorSet) {
                   return DescriptorSet(descriptorSet);
                 });
  return descriptorSets;
}

}  // namespace vk
