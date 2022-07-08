#ifndef SRC_MEMORY_MEMORYBLOCK_H_
#define SRC_MEMORY_MEMORYBLOCK_H_

#include "core/types.h"

struct MemoryBlock {
  u64 offset;
  u64 size;
};

#endif  // SRC_MEMORY_MEMORYBLOCK_H_
