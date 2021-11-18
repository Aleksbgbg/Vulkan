#include "DescriptorConfiguration.h"

void EmptyDescriptorConfiguration::ConfigureDescriptorPoolSizes(
    std::vector<VkDescriptorPoolSize>& poolSizes) const {}

std::optional<DescriptorSetLayout>
EmptyDescriptorConfiguration::ConfigureActorDescriptorSet(
    const DescriptorSetLayoutFactory& descriptorSetLayoutFactory) const {
  return std::nullopt;
}
