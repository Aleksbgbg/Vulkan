#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGESUBRESOURCELAYERS_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGESUBRESOURCELAYERS_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(ImageSubresourceLayersBuilder,
                           VkImageSubresourceLayers)
STRUCTURE_SETTER(AspectMask, VkImageAspectFlags, aspectMask)
STRUCTURE_SETTER(MipLevel, u32, mipLevel)
STRUCTURE_SETTER(BaseArrayLayer, u32, baseArrayLayer)
STRUCTURE_SETTER(LayerCount, u32, layerCount)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_IMAGESUBRESOURCELAYERS_H_
