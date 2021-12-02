#ifndef VULKAN_SRC_GAME_RENDERS_DESCRIPTORCONFIGURATION_H_
#define VULKAN_SRC_GAME_RENDERS_DESCRIPTORCONFIGURATION_H_

#include <vulkan/vulkan.h>

#include <optional>

#include "DescriptorSetLayoutFactory.h"
#include "util/types.h"
#include "vulkan/DescriptorSetLayout.h"
#include "vulkan/VirtualDevice.h"

class DescriptorConfiguration {
 public:
  virtual ~DescriptorConfiguration() = default;

  virtual std::optional<DescriptorSetLayout> ConfigureActorDescriptorSet(
      const DescriptorSetLayoutFactory& descriptorSetLayoutFactory) const = 0;
};

class EmptyDescriptorConfiguration : public DescriptorConfiguration {
 public:
  std::optional<DescriptorSetLayout> ConfigureActorDescriptorSet(
      const DescriptorSetLayoutFactory& descriptorSetLayoutFactory)
      const override;
};

#endif  // VULKAN_SRC_GAME_RENDERS_DESCRIPTORCONFIGURATION_H_
