#ifndef VULKAN_SRC_MEMORY_MEMORYALLOCATION_H_
#define VULKAN_SRC_MEMORY_MEMORYALLOCATION_H_

#include <vulkan/vulkan.h>

struct MemoryAllocation {
  VkDeviceSize size;
  VkDeviceSize alignment;
};

#endif  // VULKAN_SRC_MEMORY_MEMORYALLOCATION_H_
