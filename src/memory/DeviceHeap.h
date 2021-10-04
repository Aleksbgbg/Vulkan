#ifndef VULKAN_SRC_MEMORY_DEVICEHEAP_H
#define VULKAN_SRC_MEMORY_DEVICEHEAP_H

#include <memory>
#include <vector>

#include "types.h"
#include "vulkan/DeviceMemory.h"
#include "MemoryAllocation.h"

class VirtualDevice;

struct MemoryAllocation {
  VkDeviceSize size;
  VkDeviceSize alignment;
};

struct MemoryBlock {
  DeviceMemory* memory;

  u32 allocationIndex;
  u32 offset;
  u32 size;
};

class MemoryBinding {
  friend class DeviceHeap;
public:
  MemoryBinding();
  ~MemoryBinding();

  MemoryBinding(MemoryBinding&) = delete;
  MemoryBinding(MemoryBinding&& other) noexcept;

  MemoryBinding& operator=(MemoryBinding&) = delete;
  MemoryBinding& operator=(MemoryBinding&& other) noexcept;

  DeviceMemory& GetMemory() const;

private:
  MemoryBinding(DeviceHeap* heap, const MemoryBlock memoryBlock);

  DeviceHeap* heap;
  MemoryBlock memoryBlock;
};

struct ReservedMemory {
  MemoryBinding memoryBinding;
  VkDeviceSize offset;
};

class DeviceHeap {
  friend MemoryBinding;
public:
  DeviceHeap() = default;
  explicit DeviceHeap(VkDevice device, const u32 memoryTypeIndex);

  ReservedMemory ReserveMemoryBlock(const VkMemoryRequirements& memoryRequirements);

private:
  struct AvailableBlock {
    MemoryBlock memoryBlock;

    std::shared_ptr<AvailableBlock> previous;
    std::shared_ptr<AvailableBlock> next;
  };

  void FreeMemoryBlock(const MemoryBlock memoryBlock);

  void Enlarge();
  void UpdateLastBlock(const std::shared_ptr<AvailableBlock>& newLastBlock);

  std::optional<std::shared_ptr<AvailableBlock>> TryFindAvailableBlock(const MemoryAllocation requestedAllocation);
  MemoryBlock ReserveBlock(const std::shared_ptr<AvailableBlock>& block, const MemoryAllocation requestedAllocation);

private:
  VkDevice device;

  u32 memoryTypeIndex;
  u32 totalSize;

  std::shared_ptr<AvailableBlock> firstAvailableBlock;
  std::shared_ptr<AvailableBlock> lastAvailableBlock;

  std::vector<std::shared_ptr<DeviceMemory>> allocatedMemory;
};

#endif // VULKAN_SRC_MEMORY_DEVICEHEAP_H
