#include "PipelineLayout.h"

#include <algorithm>

#include "error.h"

PipelineLayout::PipelineLayout(
    VkDevice device,
    const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts)
    : PipelineLayout(device, PipelineLayoutCreateInfoBuilder().Reference(),
                     descriptorSetLayouts) {}

PipelineLayout::PipelineLayout(
    VkDevice device, PipelineLayoutCreateInfoBuilder& infoBuilder,
    const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts)
    : device(device) {
  std::vector<VkDescriptorSetLayout> descriptorSetLayoutsRaw(
      descriptorSetLayouts.size());
  std::transform(descriptorSetLayouts.begin(), descriptorSetLayouts.end(),
                 descriptorSetLayoutsRaw.begin(),
                 [](const DescriptorSetLayout* const layout) {
                   return layout->descriptorSetLayout;
                 });
  PROCEED_ON_VALID_RESULT(vkCreatePipelineLayout(
      device,
      infoBuilder.SetSetLayoutCount(descriptorSetLayoutsRaw.size())
          .SetPSetLayouts(descriptorSetLayoutsRaw.data())
          .Build(),
      nullptr, &pipelineLayout));
}

PipelineLayout::~PipelineLayout() {
  if (pipelineLayout != nullptr) {
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
  }
}
