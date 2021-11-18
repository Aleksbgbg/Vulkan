#ifndef VULKAN_SRC_GAME_RENDERS_DESCRIPTORSETLAYOUTFACTORY_H
#define VULKAN_SRC_GAME_RENDERS_DESCRIPTORSETLAYOUTFACTORY_H

#include "vulkan/DescriptorSetLayout.h"

class DescriptorSetLayoutFactory {
 public:
  virtual ~DescriptorSetLayoutFactory() = default;

  virtual DescriptorSetLayout CreateDescriptorSetLayout(
      const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_DESCRIPTORSETLAYOUTFACTORY_H
