#include "DeviceMemoryAllocator.h"

#include <stdexcept>

static constexpr u32 InitialAllocationSize = 1u * 1024u * 1024u;

DeviceMemoryAllocator::DeviceMemoryAllocator(VkDevice device, VkPhysicalDeviceMemoryProperties* const memoryProperties)
  : memoryIndexHeap(), device(device), memoryProperties(memoryProperties) {
  for (u32 memoryIndex = 0; memoryIndex < memoryIndexHeap.size(); ++memoryIndex) {
    memoryIndexHeap[memoryIndex] = DeviceHeap(device, memoryIndex);
  }
}

ReservedMemory DeviceMemoryAllocator::BindMemory(
    const Buffer& buffer, const VkMemoryPropertyFlags requiredProperties) {
  ReservedMemory reservedMemory = ReserveMemoryBlock(buffer.GetMemoryRequirements(), requiredProperties);
  reservedMemory.memoryBinding.GetMemory().Bind(buffer, reservedMemory.offset);

  return reservedMemory;
}

ReservedMemory DeviceMemoryAllocator::BindMemory(
    const Image& image, const VkMemoryPropertyFlags requiredProperties) {
  ReservedMemory reservedMemory = ReserveMemoryBlock(image.GetMemoryRequirements(), requiredProperties);
  reservedMemory.memoryBinding.GetMemory().Bind(image, reservedMemory.offset);

  return reservedMemory;
}

ReservedMemory DeviceMemoryAllocator::ReserveMemoryBlock(
    const VkMemoryRequirements& memoryRequirements, const VkMemoryPropertyFlags requiredProperties) {
  const std::optional<u32> memoryTypeIndex =
      DeviceMemory::FindSuitableMemoryTypeIndex(
          *memoryProperties, requiredProperties, memoryRequirements.memoryTypeBits);

  if (!memoryTypeIndex.has_value()) {
    throw std::runtime_error("Could not find suitable GPU memory to allocate.");
  }

  return memoryIndexHeap[memoryTypeIndex.value()].ReserveMemoryBlock(memoryRequirements);
}
