#ifndef VULKAN_SRC_MEMORY_ALLOCATOR_H_
#define VULKAN_SRC_MEMORY_ALLOCATOR_H_

#include <memory>

#include "util/types.h"

class Allocator {
 public:
  class MemoryObject {
   public:
    virtual ~MemoryObject() = default;
  };

  virtual ~Allocator() = default;

  virtual u64 MemoryObjectSize() const = 0;
  virtual MemoryObject* Allocate(void* memoryObjectMemory, u64 size) = 0;
};

#endif  // VULKAN_SRC_MEMORY_ALLOCATOR_H_
