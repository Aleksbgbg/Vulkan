#ifndef VULKAN_SRC_VULKAN_STRUCTURES_IMAGESUBRESOURCELAYERS_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_IMAGESUBRESOURCELAYERS_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(ImageSubresourceLayersBuilder,
                           VkImageSubresourceLayers)
STRUCTURE_SETTER(AspectMask, VkImageAspectFlags, aspectMask)
STRUCTURE_SETTER(MipLevel, u32, mipLevel)
STRUCTURE_SETTER(BaseArrayLayer, u32, baseArrayLayer)
STRUCTURE_SETTER(LayerCount, u32, layerCount)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_IMAGESUBRESOURCELAYERS_H_
