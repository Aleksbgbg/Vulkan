#ifndef VULKAN_SRC_MEMORY_MEMORYBLOCK_H_
#define VULKAN_SRC_MEMORY_MEMORYBLOCK_H_

#include <vulkan/vulkan.h>

struct MemoryBlock {
  VkDeviceSize offset;
  VkDeviceSize size;
};

#endif  // VULKAN_SRC_MEMORY_MEMORYBLOCK_H_
