#include "ReservedBlock.h"

#include "DeviceHeap.h"
#include "MemoryBlock.h"

MemoryBlock ReservedBlock::GetMemoryBlock() const {
  return MemoryBlock{.offset = info.availableOffset,
                     .size = info.availableSize};
}

MemoryObject* ReservedBlock::GetMemoryObject() const {
  return memory;
}

ReservedBlock::ReservedBlock() : heap(nullptr) {}

ReservedBlock::ReservedBlock(DeviceHeap* heap, MemoryObject* memory,
                             AllocationInfo info)
    : heap(heap), memory(memory), info(info) {}

ReservedBlock::~ReservedBlock() {
  if (heap != nullptr) {
    heap->Return({.allocationIndex = info.allocationIndex,
                  .offset = info.allocationOffset,
                  .size = info.allocationSize});
  }
}

ReservedBlock::ReservedBlock(ReservedBlock&& other) noexcept
    : heap(other.heap), memory(other.memory), info(other.info) {
  other.heap = nullptr;
}

ReservedBlock& ReservedBlock::operator=(ReservedBlock&& other) noexcept {
  heap = other.heap;
  memory = other.memory;
  info = other.info;
  other.heap = nullptr;
  return *this;
}
