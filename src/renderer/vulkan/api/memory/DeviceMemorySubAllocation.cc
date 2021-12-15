#include "DeviceMemorySubAllocation.h"

DeviceMemorySubAllocation::DeviceMemorySubAllocation(
    const DeviceMemory* const memory, ReservedBlock reservedBlock)
    : memory_(memory),
      reservedBlock_(std::move(reservedBlock)),
      allocatedMemoryOffset_(reservedBlock.GetMemoryBlock().offset) {}

void DeviceMemorySubAllocation::Bind(const Buffer& buffer) const {
  memory_->Bind(buffer, allocatedMemoryOffset_);
}

void DeviceMemorySubAllocation::Bind(const Image& image) const {
  memory_->Bind(image, allocatedMemoryOffset_);
}

void* DeviceMemorySubAllocation::Map(const VkDeviceSize offset,
                                     const VkDeviceSize size) const {
  return memory_->Map(allocatedMemoryOffset_ + offset, size);
}

void DeviceMemorySubAllocation::Unmap() const {
  memory_->Unmap();
}

void DeviceMemorySubAllocation::MapCopy(const void* const data,
                                        const VkDeviceSize size) const {
  memory_->MapCopy(data, allocatedMemoryOffset_, size);
}

void DeviceMemorySubAllocation::FlushMappedMemoryRange(
    const VkDeviceSize offset, const VkDeviceSize size) const {
  memory_->FlushMappedMemoryRange(allocatedMemoryOffset_ + offset, size);
}
