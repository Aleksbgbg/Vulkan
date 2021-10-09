#include "DescriptorPool.h"

#include <algorithm>
#include <iterator>

#include "error.h"

DescriptorPool::DescriptorPool(VkDevice device,
                               DescriptorPoolCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(vkCreateDescriptorPool(device, infoBuilder.Build(),
                                                 nullptr, &descriptorPool));
}

DescriptorPool::~DescriptorPool() {
  if (descriptorPool != nullptr) {
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
  }
}

std::vector<DescriptorSet> DescriptorPool::AllocateDescriptorSets(
    DescriptorSetLayout& descriptorSetLayout, const u32 count) const {
  std::vector<VkDescriptorSetLayout> rawLayouts(
      count, descriptorSetLayout.descriptorSetLayout);
  VkDescriptorSetAllocateInfo allocateInfo =
      DescriptorSetAllocateInfoBuilder()
          .SetDescriptorPool(descriptorPool)
          .SetDescriptorSetCount(count)
          .SetPSetLayouts(rawLayouts.data());
  std::vector<VkDescriptorSet> rawDescriptorSets(count);
  PROCEED_ON_VALID_RESULT(
      vkAllocateDescriptorSets(device, &allocateInfo, rawDescriptorSets.data()))
  std::vector<DescriptorSet> descriptorSets;
  std::transform(rawDescriptorSets.begin(), rawDescriptorSets.end(),
                 std::back_inserter(descriptorSets),
                 [](VkDescriptorSet descriptorSet) {
                   return DescriptorSet(descriptorSet);
                 });
  return descriptorSets;
}
