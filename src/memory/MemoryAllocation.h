#ifndef VULKAN_SRC_MEMORY_MEMORYALLOCATION_H
#define VULKAN_SRC_MEMORY_MEMORYALLOCATION_H

#include <vulkan/vulkan.h>

struct MemoryAllocation {
  VkDeviceSize size;
  VkDeviceSize alignment;
};

#endif  // VULKAN_SRC_MEMORY_MEMORYALLOCATION_H
