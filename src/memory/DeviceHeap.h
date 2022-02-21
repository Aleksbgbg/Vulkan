#ifndef VULKAN_SRC_MEMORY_DEVICEHEAP_H_
#define VULKAN_SRC_MEMORY_DEVICEHEAP_H_

#include <memory>
#include <vector>

#include "AllocatedBlock.h"
#include "Allocator.h"
#include "MemoryAllocation.h"
#include "MemoryBlock.h"
#include "ReservedBlock.h"
#include "core/types.h"

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
    std::unique_ptr<Allocator::MemoryObject> memory;
  };

  void Return(const AllocatedBlock block);

 private:
  u64 initialAllocationSize_;
  float enlargementFactor_;
  Allocator* allocator_;

  u32 enlargementIndex_;
  std::vector<AllocatedMemory> allocations_;
};

#endif  // VULKAN_SRC_MEMORY_DEVICEHEAP_H_
