#ifndef VULKAN_SRC_VULKAN_STRUCTURES_IMAGESUBRESOURCELAYERS_H
#define VULKAN_SRC_VULKAN_STRUCTURES_IMAGESUBRESOURCELAYERS_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER_NO_TYPE(ImageSubresourceLayersBuilder,
                          VkImageSubresourceLayers)
STRUCTURE_SETTER(ImageSubresourceLayersBuilder, AspectMask, VkImageAspectFlags,
                 aspectMask)
STRUCTURE_SETTER(ImageSubresourceLayersBuilder, MipLevel, u32, mipLevel)
STRUCTURE_SETTER(ImageSubresourceLayersBuilder, BaseArrayLayer, u32,
                 baseArrayLayer)
STRUCTURE_SETTER(ImageSubresourceLayersBuilder, LayerCount, u32, layerCount)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_IMAGESUBRESOURCELAYERS_H
