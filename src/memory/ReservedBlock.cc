#include "ReservedBlock.h"

#include "DeviceHeap.h"
#include "MemoryBlock.h"

MemoryBlock ReservedBlock::GetMemoryBlock() const {
  return MemoryBlock{.offset = info_.availableOffset,
                     .size = info_.availableSize};
}

ReservedBlock::ReservedBlock() : heap_(nullptr) {}

ReservedBlock::ReservedBlock(DeviceHeap* heap, Allocator::MemoryObject* memory,
                             AllocationInfo info)
    : heap_(heap), memory_(memory), info_(info) {}

ReservedBlock::~ReservedBlock() {
  if (heap_ != nullptr) {
    heap_->Return({.allocationIndex = info_.allocationIndex,
                   .offset = info_.allocationOffset,
                   .size = info_.allocationSize});
  }
}

ReservedBlock::ReservedBlock(ReservedBlock&& other) noexcept
    : heap_(other.heap_), memory_(other.memory_), info_(other.info_) {
  other.heap_ = nullptr;
}

ReservedBlock& ReservedBlock::operator=(ReservedBlock&& other) noexcept {
  heap_ = other.heap_;
  memory_ = other.memory_;
  info_ = other.info_;
  other.heap_ = nullptr;
  return *this;
}
