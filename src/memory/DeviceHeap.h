#ifndef VULKAN_SRC_MEMORY_DEVICEHEAP_H
#define VULKAN_SRC_MEMORY_DEVICEHEAP_H

#include <memory>
#include <vector>

#include "AllocatedBlock.h"
#include "Allocator.h"
#include "MemoryAllocation.h"
#include "MemoryBlock.h"
#include "ReservedBlock.h"
#include "util/types.h"

class DeviceHeap {
  friend ReservedBlock;

 public:
  DeviceHeap();
  DeviceHeap(u64 initialAllocationSize, Allocator* allocator);
  DeviceHeap(u64 initialAllocationSize, float enlargementFactor,
             Allocator* allocator);

  ReservedBlock ReserveMemory(const MemoryAllocation requestedAllocation);

 private:
  struct AllocationChain {
    AllocatedBlock block;

    std::unique_ptr<AllocationChain> next;
  };

  class AllocationList {
   public:
    AllocationChain* First();

    void Add(const AllocatedBlock block);
    void InsertAfter(AllocationChain* previous, const AllocatedBlock block);
    void Remove(AllocationChain* block, AllocationChain* previous);

   private:
    std::unique_ptr<AllocationChain> first;
    AllocationChain* last;
  };

  struct AllocatedMemory {
    u64 allocationIndex;
    AllocationList list;
    std::unique_ptr<MemoryObject> memory;
  };

  void Return(const AllocatedBlock block);

 private:
  u64 initialAllocationSize;
  float enlargementFactor;
  Allocator* allocator;

  u32 enlargementIndex;
  std::vector<AllocatedMemory> allocations;
};

#endif  // VULKAN_SRC_MEMORY_DEVICEHEAP_H