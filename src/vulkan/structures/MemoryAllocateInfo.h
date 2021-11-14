#ifndef VULKAN_SRC_VULKAN_STRUCTURES_MEMORYALLOCATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_MEMORYALLOCATEINFO_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(MemoryAllocateInfoBuilder, VkMemoryAllocateInfo,
                  VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO)
STRUCTURE_SETTER(AllocationSize, const VkDeviceSize, allocationSize)
STRUCTURE_SETTER(MemoryTypeIndex, const u32, memoryTypeIndex)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_MEMORYALLOCATEINFO_H
