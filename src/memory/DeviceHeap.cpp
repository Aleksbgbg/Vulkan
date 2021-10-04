#include "DeviceHeap.h"

#include "vulkan/VirtualDevice.h"

static constexpr u32 InitialAllocationSize = 1u * 1024u * 1024u;

MemoryBinding::MemoryBinding() : heap(nullptr) {}

MemoryBinding::MemoryBinding(
    DeviceHeap* heap, const MemoryBlock memoryBlock)
    : heap(heap), memoryBlock(memoryBlock) {}

MemoryBinding::~MemoryBinding() {
  if (heap != nullptr) {
    heap->FreeMemoryBlock(memoryBlock);
  }
}

MemoryBinding::MemoryBinding(MemoryBinding&& other) noexcept
    : heap(other.heap), memoryBlock(other.memoryBlock) {
  other.heap = nullptr;
}

MemoryBinding& MemoryBinding::operator=(MemoryBinding&& other) noexcept {
  heap = other.heap;
  memoryBlock = other.memoryBlock;
  other.heap = nullptr;
  return *this;
}

DeviceMemory& MemoryBinding::GetMemory() const {
  return *memoryBlock.memory;
}

DeviceHeap::DeviceHeap(VkDevice device, const u32 memoryTypeIndex)
    : device(device), memoryTypeIndex(memoryTypeIndex) {}

ReservedMemory DeviceHeap::ReserveMemoryBlock(const VkMemoryRequirements& memoryRequirements) {
  MemoryAllocation requestedAllocation = {
      .size = memoryRequirements.size,
      .alignment = memoryRequirements.alignment
  };

  std::optional<std::shared_ptr<AvailableBlock>> availableAllocation = TryFindAvailableBlock(requestedAllocation);

  while (!availableAllocation.has_value()) {
    Enlarge();
    availableAllocation = TryFindAvailableBlock(requestedAllocation);
  }

  const std::shared_ptr<AvailableBlock>& allocationBlock = availableAllocation.value();
  MemoryBlock reservedBlock = ReserveBlock(allocationBlock, requestedAllocation);
  return ReservedMemory{
      .memoryBinding = MemoryBinding(this, reservedBlock),
      .offset = reservedBlock.offset
  };
}

void DeviceHeap::Enlarge() {
  const u32 allocationSize = totalSize == 0 ? InitialAllocationSize : totalSize;
  const u32 allocationIndex = allocatedMemory.size();
  allocatedMemory.emplace_back(
      std::make_shared<DeviceMemory>(
          DeviceMemory(
              device,
              MemoryAllocateInfoBuilder()
                  .SetMemoryTypeIndex(memoryTypeIndex)
                  .SetAllocationSize(allocationSize))));
  totalSize += allocationSize;
  const std::shared_ptr<AvailableBlock> newAllocationAvailableBlock =
      std::make_shared<AvailableBlock>(
          AvailableBlock{
              .memoryBlock = MemoryBlock{
                  .memory = allocatedMemory[allocationIndex].get(),
                  .allocationIndex = allocationIndex,
                  .offset = 0,
                  .size = allocationSize
              }
          });
  if (firstAvailableBlock == nullptr) {
    firstAvailableBlock = newAllocationAvailableBlock;
  }
  UpdateLastBlock(newAllocationAvailableBlock);
}

void DeviceHeap::UpdateLastBlock(const std::shared_ptr<AvailableBlock>& newLastBlock) {
  const std::shared_ptr<AvailableBlock> nextToLastBlock = lastAvailableBlock;
  lastAvailableBlock = newLastBlock;
  if (nextToLastBlock != nullptr) {
    newLastBlock->previous = nextToLastBlock;
    nextToLastBlock->next = newLastBlock;
  }
}

u32 Align(const u32 address, const u32 alignment) {
  const u32 currentOffset = address % alignment;
  const u32 requiredPadding = (alignment - currentOffset) % alignment;
  return address + requiredPadding;
}

bool IsSuitableMemoryBlock(const MemoryBlock& block, const MemoryAllocation requestedAllocation) {
  const u32 alignedOffset = Align(block.offset, requestedAllocation.alignment);
  const u32 requiredSizeAligned = (alignedOffset - block.offset) + requestedAllocation.size;
  return block.size > requiredSizeAligned;
}

std::optional<std::shared_ptr<DeviceHeap::AvailableBlock>> DeviceHeap::TryFindAvailableBlock(
    const MemoryAllocation requestedAllocation) {
  std::shared_ptr<AvailableBlock> targetBlock = firstAvailableBlock;

  if (targetBlock == nullptr) {
    return std::nullopt;
  }

  while (!IsSuitableMemoryBlock(targetBlock->memoryBlock, requestedAllocation)) {
    if (targetBlock->next == nullptr) {
      return std::nullopt;
    }

    targetBlock = targetBlock->next;
  }

  return targetBlock;
}

MemoryBlock DeviceHeap::ReserveBlock(
    const std::shared_ptr<AvailableBlock>& block, const MemoryAllocation requestedAllocation) {
  const u32 alignedOffset = Align(block->memoryBlock.offset, requestedAllocation.alignment);
  const u32 alignedSize = (alignedOffset - block->memoryBlock.offset) + requestedAllocation.size;

  MemoryBlock reservedBlock = block->memoryBlock;
  reservedBlock.offset = alignedOffset;
  reservedBlock.size = alignedSize;

  if (block->memoryBlock.size > alignedSize) {
    block->memoryBlock.offset += alignedSize;
    block->memoryBlock.size -= alignedSize;
  } else {
    const std::shared_ptr<AvailableBlock>& previous = block->previous;
    const std::shared_ptr<AvailableBlock>& next = block->next;

    if (previous != nullptr) {
      previous->next = next;
    }
    if (next != nullptr) {
      next->previous = previous;
    }

    if (firstAvailableBlock == block) {
      firstAvailableBlock = next;
    }
    if (lastAvailableBlock == block) {
      lastAvailableBlock = previous;
    }
  }

  return reservedBlock;
}

void DeviceHeap::FreeMemoryBlock(const MemoryBlock memoryBlock) {
  std::shared_ptr<AvailableBlock> allocationBlock = firstAvailableBlock;

  if (allocationBlock->memoryBlock.allocationIndex > memoryBlock.allocationIndex) {
    firstAvailableBlock = std::make_shared<AvailableBlock>(AvailableBlock{
        .memoryBlock = memoryBlock
    });
    firstAvailableBlock->next = allocationBlock;
    allocationBlock->previous = firstAvailableBlock;
    return;
  }

  while (allocationBlock->memoryBlock.allocationIndex != memoryBlock.allocationIndex) {
    if (allocationBlock->next == nullptr) {
      allocationBlock->next = std::make_shared<AvailableBlock>(AvailableBlock{
          .memoryBlock = memoryBlock
      });
      return;
    }

    if (allocationBlock->next->memoryBlock.allocationIndex > memoryBlock.allocationIndex) {
      const std::shared_ptr<AvailableBlock>& previous = allocationBlock;
      const std::shared_ptr<AvailableBlock>& next = allocationBlock->next;
      const std::shared_ptr<AvailableBlock> current = std::make_shared<AvailableBlock>(AvailableBlock{
          .memoryBlock = memoryBlock
      });

      previous->next = current;
      current->previous = previous;
      current->next = next;
      next->previous = current;
      return;
    }

    allocationBlock = allocationBlock->next;
  }

  while ((allocationBlock->memoryBlock.offset + allocationBlock->memoryBlock.size) < memoryBlock.offset) {
    allocationBlock = allocationBlock->next;
  }

  if ((allocationBlock->memoryBlock.offset + allocationBlock->memoryBlock.size) == memoryBlock.offset) {
    allocationBlock->memoryBlock.size += memoryBlock.size;

    if ((allocationBlock->memoryBlock.offset + allocationBlock->memoryBlock.size) == allocationBlock->next->memoryBlock.offset) {
      allocationBlock->memoryBlock.size += allocationBlock->next->memoryBlock.size;
      allocationBlock->next = allocationBlock->next->next;
      allocationBlock->next->previous = allocationBlock;
    }

    return;
  }

  if ((memoryBlock.offset + memoryBlock.size) == allocationBlock->memoryBlock.offset) {
    allocationBlock->memoryBlock.offset -= memoryBlock.size;
    allocationBlock->memoryBlock.size += memoryBlock.size;
    return;
  }

  const std::shared_ptr<AvailableBlock>& previous = allocationBlock;
  const std::shared_ptr<AvailableBlock>& next = allocationBlock->next;
  const std::shared_ptr<AvailableBlock> current = std::make_shared<AvailableBlock>(AvailableBlock{
      .memoryBlock = memoryBlock
  });

  previous->next = current;
  current->previous = previous;
  current->next = next;
  next->previous = current;
}
