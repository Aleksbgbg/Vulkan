#ifndef SRC_MEMORY_MEMORYALLOCATION_H_
#define SRC_MEMORY_MEMORYALLOCATION_H_

#include "core/types.h"

struct MemoryAllocation {
  u64 size;
  u64 alignment;
};

#endif  // SRC_MEMORY_MEMORYALLOCATION_H_
