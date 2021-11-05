#ifndef VULKAN_SRC_GAME_RENDERS_DESCRIPTORCONFIGURATION_H
#define VULKAN_SRC_GAME_RENDERS_DESCRIPTORCONFIGURATION_H

#include <vulkan/vulkan.h>

#include <optional>

#include "util/types.h"
#include "vulkan/DescriptorSetLayout.h"
#include "vulkan/VirtualDevice.h"

class DescriptorConfiguration {
 public:
  virtual ~DescriptorConfiguration() = default;

  virtual void ConfigureDescriptorPoolSizes(
      std::vector<VkDescriptorPoolSize>& poolSizes) const = 0;
  virtual std::optional<DescriptorSetLayout> ConfigureActorDescriptorSet(
      const VirtualDevice& virtualDevice) const = 0;
};

class EmptyDescriptorConfiguration : public DescriptorConfiguration {
 public:
  void ConfigureDescriptorPoolSizes(
      std::vector<VkDescriptorPoolSize>& poolSizes) const override;
  std::optional<DescriptorSetLayout> ConfigureActorDescriptorSet(
      const VirtualDevice& virtualDevice) const override;
};

#endif  // VULKAN_SRC_GAME_RENDERS_DESCRIPTORCONFIGURATION_H
