#ifndef VULKAN_SRC_MEMORY_RESERVEDBLOCK_H
#define VULKAN_SRC_MEMORY_RESERVEDBLOCK_H

#include "AllocatedBlock.h"
#include "MemoryBlock.h"
#include "MemoryObject.h"

class DeviceHeap;

class ReservedBlock {
  friend DeviceHeap;

 public:
  MemoryBlock GetMemoryBlock() const;

  template <typename T>
  T& GetMemoryObjectAs() const {
    return *reinterpret_cast<T*>(memory);
  }

  ReservedBlock();
  ~ReservedBlock();

  ReservedBlock(ReservedBlock&) = delete;
  ReservedBlock(ReservedBlock&& other) noexcept;

  ReservedBlock& operator=(ReservedBlock&) = delete;
  ReservedBlock& operator=(ReservedBlock&& other) noexcept;

 private:
  struct AllocationInfo {
    u64 allocationIndex;
    u64 allocationOffset;
    u64 allocationSize;

    u64 availableOffset;
    u64 availableSize;
  };

  ReservedBlock(DeviceHeap* heap, MemoryObject* memory, AllocationInfo info);

 private:
  DeviceHeap* heap;
  MemoryObject* memory;
  AllocationInfo info;
};

#endif  // VULKAN_SRC_MEMORY_RESERVEDBLOCK_H
