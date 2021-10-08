#ifndef VULKAN_SRC_VULKAN_STRUCTURES_MEMORYALLOCATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_MEMORYALLOCATEINFO_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(MemoryAllocateInfoBuilder, VkMemoryAllocateInfo,
                  VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO)
STRUCTURE_SETTER(MemoryAllocateInfoBuilder, AllocationSize, const VkDeviceSize,
                 allocationSize)
STRUCTURE_SETTER(MemoryAllocateInfoBuilder, MemoryTypeIndex, const u32,
                 memoryTypeIndex)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_MEMORYALLOCATEINFO_H
