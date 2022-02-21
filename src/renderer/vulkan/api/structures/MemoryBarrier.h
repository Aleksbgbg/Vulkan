#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_MEMORYBARRIER_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_MEMORYBARRIER_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(MemoryBarrierBuilder, VkMemoryBarrier,
                  VK_STRUCTURE_TYPE_MEMORY_BARRIER)
STRUCTURE_SETTER(SrcAccessMask, VkAccessFlags, srcAccessMask)
STRUCTURE_SETTER(DstAccessMask, VkAccessFlags, dstAccessMask)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_MEMORYBARRIER_H_
