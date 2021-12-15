#ifndef VULKAN_SRC_MEMORY_MEMORYALLOCATION_H_
#define VULKAN_SRC_MEMORY_MEMORYALLOCATION_H_

#include "util/types.h"

struct MemoryAllocation {
  u64 size;
  u64 alignment;
};

#endif  // VULKAN_SRC_MEMORY_MEMORYALLOCATION_H_
