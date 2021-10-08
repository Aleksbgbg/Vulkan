#ifndef VULKAN_SRC_MEMORY_MEMORYBLOCK_H
#define VULKAN_SRC_MEMORY_MEMORYBLOCK_H

#include <vulkan/vulkan.h>

struct MemoryBlock {
  VkDeviceSize offset;
  VkDeviceSize size;
};

#endif  // VULKAN_SRC_MEMORY_MEMORYBLOCK_H
