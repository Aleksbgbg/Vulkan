#include "DeviceMemoryAllocator.h"

#include <stdexcept>

static constexpr u32 InitialAllocationSize = 1u * 1024u * 1024u;

struct DeviceMemoryObject : public Allocator::MemoryObject {
  DeviceMemoryObject(DeviceMemory deviceMemory)
      : deviceMemory(std::move(deviceMemory)) {}

  DeviceMemory deviceMemory;
};

DeviceMemoryAllocator::HeapAllocator::HeapAllocator(
    VirtualDevice* virtualDevice, u32 memoryTypeIndex)
    : virtualDevice_(virtualDevice), memoryTypeIndex_(memoryTypeIndex) {}

u64 DeviceMemoryAllocator::HeapAllocator::MemoryObjectSize() const {
  return sizeof(DeviceMemoryObject);
}

Allocator::MemoryObject* DeviceMemoryAllocator::HeapAllocator::Allocate(
    void* memoryObjectMemory, u64 size) {
  return new (memoryObjectMemory) DeviceMemoryObject(
      virtualDevice_->AllocateDeviceMemory(memoryTypeIndex_, size));
}

DeviceMemoryAllocator::DeviceMemoryAllocator(
    VirtualDevice* virtualDevice,
    const VkPhysicalDeviceMemoryProperties* const memoryProperties)
    : virtualDevice_(virtualDevice),
      memoryProperties_(memoryProperties),
      heapsPerMemoryIndex_(memoryProperties->memoryTypeCount) {
  for (u32 memoryIndex = 0; memoryIndex < heapsPerMemoryIndex_.size();
       ++memoryIndex) {
    Heap& heap = heapsPerMemoryIndex_[memoryIndex];
    heap.allocator = HeapAllocator(virtualDevice, memoryIndex);
    heap.deviceHeap = DeviceHeap(InitialAllocationSize, &heap.allocator);
  }
}

DeviceMemorySubAllocation DeviceMemoryAllocator::BindMemory(
    const Buffer& buffer, const VkMemoryPropertyFlags requiredProperties) {
  DeviceMemorySubAllocation reservedMemory =
      ReserveMemoryBlock(buffer.GetMemoryRequirements(), requiredProperties);
  reservedMemory.Bind(buffer);
  return reservedMemory;
}

DeviceMemorySubAllocation DeviceMemoryAllocator::BindMemory(
    const Image& image, const VkMemoryPropertyFlags requiredProperties) {
  DeviceMemorySubAllocation reservedMemory =
      ReserveMemoryBlock(image.GetMemoryRequirements(), requiredProperties);
  reservedMemory.Bind(image);
  return reservedMemory;
}

DeviceMemorySubAllocation DeviceMemoryAllocator::ReserveMemoryBlock(
    const VkMemoryRequirements& memoryRequirements,
    const VkMemoryPropertyFlags requiredProperties) {
  const std::optional<u32> memoryTypeIndex =
      DeviceMemory::FindSuitableMemoryTypeIndex(
          *memoryProperties_, requiredProperties,
          memoryRequirements.memoryTypeBits);

  if (!memoryTypeIndex.has_value()) {
    throw std::runtime_error("Could not find suitable GPU memory to allocate.");
  }

  DeviceHeap& heap = heapsPerMemoryIndex_[memoryTypeIndex.value()].deviceHeap;
  ReservedBlock reservedBlock =
      heap.ReserveMemory({.size = memoryRequirements.size,
                          .alignment = memoryRequirements.alignment});

  return DeviceMemorySubAllocation(
      &reservedBlock.GetMemoryObjectAs<DeviceMemoryObject>().deviceMemory,
      std::move(reservedBlock));
}
