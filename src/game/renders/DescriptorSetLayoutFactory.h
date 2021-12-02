#ifndef VULKAN_SRC_GAME_RENDERS_DESCRIPTORSETLAYOUTFACTORY_H_
#define VULKAN_SRC_GAME_RENDERS_DESCRIPTORSETLAYOUTFACTORY_H_

#include "vulkan/DescriptorSetLayout.h"

class DescriptorSetLayoutFactory {
 public:
  virtual ~DescriptorSetLayoutFactory() = default;

  virtual DescriptorSetLayout CreateDescriptorSetLayout(
      const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_DESCRIPTORSETLAYOUTFACTORY_H_
