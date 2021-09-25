#ifndef VULKAN_SRC_VULKAN_STRUCTURES_IMAGESUBRESOURCERANGE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_IMAGESUBRESOURCERANGE_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_TYPE(ImageSubresourceRangeBuilder, VkImageSubresourceRange)
  STRUCTURE_SETTER(ImageSubresourceRangeBuilder, AspectMask, const VkImageAspectFlags, aspectMask)
  STRUCTURE_SETTER(ImageSubresourceRangeBuilder, BaseMipLevel, const u32, baseMipLevel)
  STRUCTURE_SETTER(ImageSubresourceRangeBuilder, LevelCount, const u32, levelCount)
  STRUCTURE_SETTER(ImageSubresourceRangeBuilder, BaseArrayLayer, const u32, baseArrayLayer)
  STRUCTURE_SETTER(ImageSubresourceRangeBuilder, LayerCount, const u32, layerCount)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_IMAGESUBRESOURCERANGE_H
