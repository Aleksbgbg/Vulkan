#ifndef VULKAN_SRC_MEMORY_MEMORYBLOCK_H_
#define VULKAN_SRC_MEMORY_MEMORYBLOCK_H_

#include "util/types.h"

struct MemoryBlock {
  u64 offset;
  u64 size;
};

#endif  // VULKAN_SRC_MEMORY_MEMORYBLOCK_H_
