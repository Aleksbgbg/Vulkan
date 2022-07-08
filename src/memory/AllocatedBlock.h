#ifndef SRC_MEMORY_ALLOCATEDBLOCK_H_
#define SRC_MEMORY_ALLOCATEDBLOCK_H_

#include "core/types.h"

struct AllocatedBlock {
  u64 allocationIndex;
  u64 offset;
  u64 size;

  u64 End() const {
    return offset + size;
  }
};

#endif  // SRC_MEMORY_ALLOCATEDBLOCK_H_
