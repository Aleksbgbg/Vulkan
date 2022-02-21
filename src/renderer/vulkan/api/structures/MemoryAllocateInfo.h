#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_MEMORYALLOCATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_MEMORYALLOCATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(MemoryAllocateInfoBuilder, VkMemoryAllocateInfo,
                  VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO)
STRUCTURE_SETTER(AllocationSize, const VkDeviceSize, allocationSize)
STRUCTURE_SETTER(MemoryTypeIndex, const u32, memoryTypeIndex)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_MEMORYALLOCATEINFO_H_
