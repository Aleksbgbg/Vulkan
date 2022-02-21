#ifndef VULKAN_SRC_MEMORY_ALLOCATOR_H_
#define VULKAN_SRC_MEMORY_ALLOCATOR_H_

#include <memory>

#include "core/types.h"

class Allocator {
 public:
  class MemoryObject {
   public:
    virtual ~MemoryObject() = default;
  };

  virtual ~Allocator() = default;

  virtual std::unique_ptr<MemoryObject> Allocate(u64 size) = 0;
};

#endif  // VULKAN_SRC_MEMORY_ALLOCATOR_H_
