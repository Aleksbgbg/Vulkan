#ifndef VULKAN_SRC_MEMORY_DEVICEHEAP_H_
#define VULKAN_SRC_MEMORY_DEVICEHEAP_H_

#include <memory>
#include <vector>

#include "AllocatedBlock.h"
#include "Allocator.h"
#include "BlockBasedAllocator.h"
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
  template <typename T>
  class DestructorCaller {
   public:
    DestructorCaller(T* value) : value_(value) {}

    DestructorCaller(DestructorCaller&) = delete;
    DestructorCaller(DestructorCaller&& other) noexcept : value_(other.value_) {
      other.value_ = nullptr;
    }

    DestructorCaller& operator=(DestructorCaller&) = delete;
    DestructorCaller& operator=(DestructorCaller&& other) noexcept {
      std::swap(value_, other.value_);
      return *this;
    }

    ~DestructorCaller() {
      if (value_ != nullptr) {
        value_->~T();
      }
    }

    T* get() {
      return value_;
    }

   private:
    T* value_;
  };

  template <typename T>
  class AllocationReturner {
   public:
    AllocationReturner() : value_(nullptr) {}
    AllocationReturner(BlockBasedAllocator* allocator, T&& value)
        : allocator_(allocator),
          block_(allocator->ConsumeBlock()),
          value_(new (block_.memory) T(std::forward<T>(value))) {}

    AllocationReturner(AllocationReturner&) = delete;
    AllocationReturner(AllocationReturner&& other) noexcept
        : allocator_(other.allocator_),
          block_(other.block_),
          value_(other.value_) {
      other.value_ = nullptr;
    }

    ~AllocationReturner() {
      if (value_ != nullptr) {
        value_->~T();
        allocator_->ReturnBlock(block_);
      }
    }

    AllocationReturner& operator=(AllocationReturner&) = delete;
    AllocationReturner& operator=(AllocationReturner&& other) noexcept {
      this->~AllocationReturner();
      allocator_ = other.allocator_;
      block_ = other.block_;
      value_ = other.value_;
      other.value_ = nullptr;
      return *this;
    }

    bool operator==(const T* const other) const {
      return value_ == other;
    }

    T* operator->() const {
      return value_;
    }

    T* get() {
      return value_;
    }

   private:
    BlockBasedAllocator* allocator_;
    BlockBasedAllocator::Block block_;
    T* value_;
  };

  struct AllocationChain {
    AllocatedBlock block;

    AllocationReturner<AllocationChain> next;
  };

  class AllocationList {
   public:
    AllocationList(BlockBasedAllocator& memory)
        : first(), last(nullptr), memory(&memory) {}

    AllocationChain* First();

    void Add(const AllocatedBlock block);
    void InsertAfter(AllocationChain* previous, const AllocatedBlock block);
    void Remove(AllocationChain* block, AllocationChain* previous);

   private:
    BlockBasedAllocator* memory;
    AllocationReturner<AllocationChain> first;
    AllocationChain* last;
  };

  struct AllocatedMemory {
    u64 allocationIndex;
    AllocationList list;
    DestructorCaller<Allocator::MemoryObject> memory;
  };

  void Return(const AllocatedBlock block);

 private:
  u64 initialAllocationSize_;
  float enlargementFactor_;
  Allocator* allocator_;

  BlockBasedAllocator memoryObjectBlocks_;
  BlockBasedAllocator allocationChainBlocks_;

  u32 enlargementIndex_;
  std::vector<AllocatedMemory> allocations_;
};

#endif  // VULKAN_SRC_MEMORY_DEVICEHEAP_H_
