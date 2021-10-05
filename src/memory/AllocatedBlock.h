#ifndef VULKAN_SRC_MEMORY_ALLOCATEDBLOCK_H
#define VULKAN_SRC_MEMORY_ALLOCATEDBLOCK_H

#include "types.h"

struct AllocatedBlock {
  u64 allocationIndex;
  u64 offset;
  u64 size;

  u64 End() const {
    return offset + size;
  }
};

#endif // VULKAN_SRC_MEMORY_ALLOCATEDBLOCK_H
