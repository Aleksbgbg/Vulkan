#include "DeviceHeap.h"

#include <cmath>

#include "Alignment.h"

constexpr float DefaultEnlargementFactor = 1.5f;
constexpr u32 PaddingAbsorbtionThreshold = 128;

DeviceHeap::DeviceHeap() : DeviceHeap(0, nullptr) {}

DeviceHeap::DeviceHeap(u64 initialAllocationSize, Allocator* allocator)
    : DeviceHeap(initialAllocationSize, DefaultEnlargementFactor, allocator) {}

DeviceHeap::DeviceHeap(u64 initialAllocationSize, float enlargementFactor,
                       Allocator* allocator)
    : initialAllocationSize(initialAllocationSize),
      enlargementFactor(enlargementFactor),
      allocator(allocator),
      enlargementIndex(0),
      allocations() {}

ReservedBlock DeviceHeap::ReserveMemory(
    const MemoryAllocation requestedAllocation) {
  while (true) {
    for (AllocatedMemory& allocation : allocations) {
      AllocationList& list = allocation.list;

      AllocationChain* previous = nullptr;
      for (AllocationChain* current = list.First(); current != nullptr;
           previous = current, current = current->next.get()) {
        const u32 alignedOffset =
            Align(current->block.offset, requestedAllocation.alignment);
        const u32 alignmentPadding = (alignedOffset - current->block.offset);
        const u32 alignedSize = alignmentPadding + requestedAllocation.size;

        if (current->block.size >= alignedSize) {
          const AllocatedBlock allocatedBlock{
              .allocationIndex = current->block.allocationIndex,
              .offset = alignedOffset,
              .size = requestedAllocation.size};
          ReservedBlock::AllocationInfo allocationInfo{
              .allocationIndex = current->block.allocationIndex,
              .availableOffset = alignedOffset,
              .availableSize = requestedAllocation.size};

          if (alignmentPadding < PaddingAbsorbtionThreshold) {
            if (current->block.size > alignedSize) {
              allocationInfo.allocationOffset = current->block.offset;
              allocationInfo.allocationSize = alignedSize;

              current->block.offset += alignedSize;
              current->block.size -= alignedSize;
            } else {
              allocationInfo.allocationOffset = current->block.offset;
              allocationInfo.allocationSize = current->block.size;

              list.Remove(current, previous);
            }
          } else {
            allocationInfo.allocationOffset = alignedOffset;
            allocationInfo.allocationSize = requestedAllocation.size;

            if (current->block.size > alignedSize) {
              list.Add(AllocatedBlock{
                  .allocationIndex = current->block.allocationIndex,
                  .offset = allocatedBlock.End(),
                  .size = current->block.End() - allocatedBlock.End()});
            }

            current->block.size = alignmentPadding;
          }

          return ReservedBlock(
              this, allocations[allocatedBlock.allocationIndex].memory.get(),
              allocationInfo);
        }
      }
    }

    u64 allocationSize =
        initialAllocationSize *
        std::ceil(std::pow(enlargementFactor, enlargementIndex));

    if (requestedAllocation.size > allocationSize) {
      const u64 enlargementsRequired =
          std::log(requestedAllocation.size / initialAllocationSize) /
          std::log(enlargementFactor);
      const u64 nextEnlargementIndex = enlargementsRequired + 1;
      allocationSize =
          initialAllocationSize *
          std::ceil(std::pow(enlargementFactor, nextEnlargementIndex));
      enlargementIndex = nextEnlargementIndex + 1;
    } else {
      ++enlargementIndex;
    }

    AllocationList allocationList;
    allocationList.Add(AllocatedBlock{.allocationIndex = allocations.size(),
                                      .offset = 0,
                                      .size = allocationSize});
    allocations.emplace_back(AllocatedMemory{
        .list = std::move(allocationList),
        .memory = std::move(allocator->Allocate(allocationSize))});
  }
}

void DeviceHeap::Return(const AllocatedBlock returningBlock) {
  AllocationList& allocation = allocations[returningBlock.allocationIndex].list;

  AllocationChain* previous = nullptr;
  for (AllocationChain* current = allocation.First(); current != nullptr;
       previous = current, current = current->next.get()) {
    const bool comesBeforeCurrentBlock =
        returningBlock.End() < current->block.offset;
    const bool comesRightBeforeCurrentBlock =
        returningBlock.End() == current->block.offset;
    const bool comesRightAfterCurrentBlock =
        current->block.End() == returningBlock.offset;

    if (comesBeforeCurrentBlock) {
      allocation.InsertAfter(previous, returningBlock);
      return;
    }

    if (comesRightBeforeCurrentBlock) {
      current->block.offset -= returningBlock.size;
      current->block.size += returningBlock.size;
      return;
    }

    if (comesRightAfterCurrentBlock) {
      current->block.size += returningBlock.size;

      if ((current->next != nullptr) &&
          (current->block.End() == current->next->block.offset)) {
        current->block.size += current->next->block.size;
        allocation.Remove(current->next.get(), current);
      }

      return;
    }
  }

  allocation.Add(returningBlock);
}

DeviceHeap::AllocationChain* DeviceHeap::AllocationList::First() {
  return first.get();
}

void DeviceHeap::AllocationList::Add(const AllocatedBlock block) {
  if (first == nullptr) {
    first = std::make_unique<AllocationChain>(
        AllocationChain{.block = block, .next = nullptr});
    last = first.get();
  } else {
    last->next = std::make_unique<AllocationChain>(
        AllocationChain{.block = block, .next = nullptr});
    last = last->next.get();
  }
}

void DeviceHeap::AllocationList::InsertAfter(
    DeviceHeap::AllocationChain* previous, const AllocatedBlock block) {
  if (previous == nullptr) {
    first = std::make_unique<AllocationChain>(
        AllocationChain{.block = block, .next = std::move(first)});
  } else {
    previous->next = std::make_unique<AllocationChain>(
        AllocationChain{.block = block, .next = std::move(previous->next)});
    if (last == previous) {
      last = previous->next.get();
    }
  }
}

void DeviceHeap::AllocationList::Remove(DeviceHeap::AllocationChain* block,
                                        DeviceHeap::AllocationChain* previous) {
  if (previous == nullptr) {
    first = std::move(block->next);

    if (first == nullptr) {
      last = nullptr;
    }
  } else {
    if (block->next == nullptr) {
      last = previous;
    }
    previous->next = std::move(block->next);
  }
}
