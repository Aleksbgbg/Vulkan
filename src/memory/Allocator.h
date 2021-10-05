#ifndef VULKAN_SRC_MEMORY_ALLOCATOR_H
#define VULKAN_SRC_MEMORY_ALLOCATOR_H

#include <vulkan/vulkan.h>

#include "MemoryObject.h"

class Allocator {
public:
  virtual std::unique_ptr<MemoryObject> Allocate(VkDeviceSize size) = 0;
};

#endif // VULKAN_SRC_MEMORY_ALLOCATOR_H
