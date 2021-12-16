#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_BUFFERMEMORYBARRIER_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_BUFFERMEMORYBARRIER_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(BufferMemoryBarrierBuilder, VkBufferMemoryBarrier,
                  VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER)
STRUCTURE_SETTER(SrcAccessMask, VkAccessFlags, srcAccessMask)
STRUCTURE_SETTER(DstAccessMask, VkAccessFlags, dstAccessMask)
STRUCTURE_SETTER(SrcQueueFamilyIndex, u32, srcQueueFamilyIndex)
STRUCTURE_SETTER(DstQueueFamilyIndex, u32, dstQueueFamilyIndex)
STRUCTURE_SETTER(Buffer, VkBuffer, buffer)
STRUCTURE_SETTER(Offset, VkDeviceSize, offset)
STRUCTURE_SETTER(Size, VkDeviceSize, size)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_BUFFERMEMORYBARRIER_H_