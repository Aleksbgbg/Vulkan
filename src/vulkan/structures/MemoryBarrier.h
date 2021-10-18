#ifndef VULKAN_SRC_VULKAN_STRUCTURES_MEMORYBARRIER_H
#define VULKAN_SRC_VULKAN_STRUCTURES_MEMORYBARRIER_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(MemoryBarrierBuilder, VkMemoryBarrier,
                  VK_STRUCTURE_TYPE_MEMORY_BARRIER)
STRUCTURE_SETTER(SrcAccessMask, VkAccessFlags, srcAccessMask)
STRUCTURE_SETTER(DstAccessMask, VkAccessFlags, dstAccessMask)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_MEMORYBARRIER_H
