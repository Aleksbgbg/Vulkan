#ifndef VULKAN_SRC_VULKAN_STRUCTURES_BUFFERIMAGECOPY_H
#define VULKAN_SRC_VULKAN_STRUCTURES_BUFFERIMAGECOPY_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"
#include "ImageSubresourceLayers.h"
#include "Extent3D.h"

STRUCTURE_BUILDER_NO_TYPE(BufferImageCopyBuilder, VkBufferImageCopy)
  STRUCTURE_SETTER(BufferImageCopyBuilder, BufferOffset, VkDeviceSize, bufferOffset)
  STRUCTURE_SETTER(BufferImageCopyBuilder, BufferRowLength, u32, bufferRowLength)
  STRUCTURE_SETTER(BufferImageCopyBuilder, BufferImageHeight, u32, bufferImageHeight)
  STRUCTURE_SETTER(BufferImageCopyBuilder, ImageSubresource, VkImageSubresourceLayers, imageSubresource)
  STRUCTURE_SETTER_BUILDER_OBJECT(BufferImageCopyBuilder, ImageSubresource, ImageSubresourceLayersBuilder, imageSubresource)
  STRUCTURE_SETTER(BufferImageCopyBuilder, ImageOffset, VkOffset3D, imageOffset)
  STRUCTURE_SETTER(BufferImageCopyBuilder, ImageExtent, VkExtent3D, imageExtent)
  STRUCTURE_SETTER_BUILDER_OBJECT(BufferImageCopyBuilder, ImageExtent, Extent3DBuilder, imageExtent)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_BUFFERIMAGECOPY_H
