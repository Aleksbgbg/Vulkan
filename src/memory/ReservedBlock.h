#ifndef SRC_MEMORY_RESERVEDBLOCK_H_
#define SRC_MEMORY_RESERVEDBLOCK_H_

#include "AllocatedBlock.h"
#include "Allocator.h"
#include "MemoryBlock.h"

class DeviceHeap;

class ReservedBlock {
  friend DeviceHeap;

 public:
  MemoryBlock GetMemoryBlock() const;

  template <typename T>
  T& GetMemoryObjectAs() const {
    return *reinterpret_cast<T*>(memory_);
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

  ReservedBlock(DeviceHeap* heap, Allocator::MemoryObject* memory,
                AllocationInfo info);

 private:
  DeviceHeap* heap_;
  Allocator::MemoryObject* memory_;
  AllocationInfo info_;
};

#endif  // SRC_MEMORY_RESERVEDBLOCK_H_
