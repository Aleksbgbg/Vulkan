#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGESUBRESOURCERANGE_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGESUBRESOURCERANGE_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(ImageSubresourceRangeBuilder,
                           VkImageSubresourceRange)
STRUCTURE_SETTER(AspectMask, const VkImageAspectFlags, aspectMask)
STRUCTURE_SETTER(BaseMipLevel, const u32, baseMipLevel)
STRUCTURE_SETTER(LevelCount, const u32, levelCount)
STRUCTURE_SETTER(BaseArrayLayer, const u32, baseArrayLayer)
STRUCTURE_SETTER(LayerCount, const u32, layerCount)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGESUBRESOURCERANGE_H_
