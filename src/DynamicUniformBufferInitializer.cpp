#include "DynamicUniformBufferInitializer.h"

DynamicUniformBufferInitializer::DynamicUniformBufferInitializer(
    const u32 objectCount, const VkPhysicalDeviceLimits& limits,
    const VirtualDevice& virtualDevice, DeviceMemoryAllocator& allocator)
    : objectCount(objectCount),
      limits(limits),
      virtualDevice(virtualDevice),
      allocator(allocator) {}

u32 DynamicUniformBufferInitializer::PaddedSize(const u32 elementSize) const {
  return Align(elementSize, limits.nonCoherentAtomSize);
}

BufferWithMemory DynamicUniformBufferInitializer::CreateBuffer(
    const u32 elementPaddedSize) const {
  BufferWithMemory bufferWithMemory;
  bufferWithMemory.buffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(elementPaddedSize * objectCount)
          .SetUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT));
  bufferWithMemory.memory = allocator.BindMemory(
      bufferWithMemory.buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  return bufferWithMemory;
}
