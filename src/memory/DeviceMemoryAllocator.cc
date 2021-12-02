#include "DeviceMemoryAllocator.h"

#include <stdexcept>

static constexpr u32 InitialAllocationSize = 1u * 1024u * 1024u;

struct DeviceMemoryObject : public MemoryObject {
  DeviceMemoryObject(DeviceMemory deviceMemory)
      : deviceMemory(std::move(deviceMemory)) {}

  DeviceMemory deviceMemory;
};

DeviceMemoryAllocator::HeapAllocator::HeapAllocator(
    VirtualDevice* virtualDevice, u32 memoryTypeIndex)
    : virtualDevice(virtualDevice), memoryTypeIndex(memoryTypeIndex) {}

std::unique_ptr<MemoryObject> DeviceMemoryAllocator::HeapAllocator::Allocate(
    const VkDeviceSize size) {
  return std::make_unique<DeviceMemoryObject>(
      virtualDevice->AllocateDeviceMemory(memoryTypeIndex, size));
}

DeviceMemoryAllocator::DeviceMemoryAllocator(
    VirtualDevice* virtualDevice,
    const VkPhysicalDeviceMemoryProperties* const memoryProperties)
    : virtualDevice(virtualDevice),
      memoryProperties(memoryProperties),
      heapsPerMemoryIndex(memoryProperties->memoryTypeCount) {
  for (u32 memoryIndex = 0; memoryIndex < heapsPerMemoryIndex.size();
       ++memoryIndex) {
    Heap& heap = heapsPerMemoryIndex[memoryIndex];
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
          *memoryProperties, requiredProperties,
          memoryRequirements.memoryTypeBits);

  if (!memoryTypeIndex.has_value()) {
    throw std::runtime_error("Could not find suitable GPU memory to allocate.");
  }

  DeviceHeap& heap = heapsPerMemoryIndex[memoryTypeIndex.value()].deviceHeap;
  ReservedBlock reservedBlock =
      heap.ReserveMemory({.size = memoryRequirements.size,
                          .alignment = memoryRequirements.alignment});

  return DeviceMemorySubAllocation(
      &reservedBlock.GetMemoryObjectAs<DeviceMemoryObject>().deviceMemory,
      std::move(reservedBlock));
}
