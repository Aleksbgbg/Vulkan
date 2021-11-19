#include "DescriptorConfiguration.h"

std::optional<DescriptorSetLayout>
EmptyDescriptorConfiguration::ConfigureActorDescriptorSet(
    const DescriptorSetLayoutFactory& descriptorSetLayoutFactory) const {
  return std::nullopt;
}
