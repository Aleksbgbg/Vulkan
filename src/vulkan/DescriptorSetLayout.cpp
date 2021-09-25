#include "DescriptorSetLayout.h"
#include "error.h"

DescriptorSetLayout::DescriptorSetLayout(VkDevice device, DescriptorSetLayoutCreateInfoBuilder& infoBuilder)
  : device(device) {
  PROCEED_ON_VALID_RESULT(vkCreateDescriptorSetLayout(device, infoBuilder.Build(), nullptr, &descriptorSetLayout))
}

DescriptorSetLayout::~DescriptorSetLayout() {
  if (descriptorSetLayout != nullptr) {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
  }
}
