#include "DeviceMemorySubAllocation.h"

DeviceMemorySubAllocation::DeviceMemorySubAllocation(
    const DeviceMemory* const memory, ReservedBlock reservedBlock)
    : memory(memory),
      reservedBlock(std::move(reservedBlock)),
      allocatedMemoryOffset(reservedBlock.GetMemoryBlock().offset) {}

void DeviceMemorySubAllocation::Bind(const Buffer& buffer) const {
  memory->Bind(buffer, allocatedMemoryOffset);
}

void DeviceMemorySubAllocation::Bind(const Image& image) const {
  memory->Bind(image, allocatedMemoryOffset);
}

void* DeviceMemorySubAllocation::Map(const VkDeviceSize offset,
                                     const VkDeviceSize size) const {
  return memory->Map(allocatedMemoryOffset + offset, size);
}

void DeviceMemorySubAllocation::Unmap() const {
  memory->Unmap();
}

void DeviceMemorySubAllocation::MapCopy(const void* const data,
                                        const VkDeviceSize size) const {
  memory->MapCopy(data, allocatedMemoryOffset, size);
}

void DeviceMemorySubAllocation::FlushMappedMemoryRange(
    const VkDeviceSize offset, const VkDeviceSize size) const {
  memory->FlushMappedMemoryRange(allocatedMemoryOffset + offset, size);
}
