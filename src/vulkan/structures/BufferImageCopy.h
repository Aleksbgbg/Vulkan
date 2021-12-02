#ifndef VULKAN_SRC_VULKAN_STRUCTURES_BUFFERIMAGECOPY_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_BUFFERIMAGECOPY_H_

#include <vulkan/vulkan.h>

#include "Extent3D.h"
#include "ImageSubresourceLayers.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(BufferImageCopyBuilder, VkBufferImageCopy)
STRUCTURE_SETTER(BufferOffset, VkDeviceSize, bufferOffset)
STRUCTURE_SETTER(BufferRowLength, u32, bufferRowLength)
STRUCTURE_SETTER(BufferImageHeight, u32, bufferImageHeight)
STRUCTURE_SETTER(ImageSubresource, VkImageSubresourceLayers, imageSubresource)
STRUCTURE_SETTER(ImageOffset, VkOffset3D, imageOffset)
STRUCTURE_SETTER(ImageExtent, VkExtent3D, imageExtent)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_BUFFERIMAGECOPY_H_
