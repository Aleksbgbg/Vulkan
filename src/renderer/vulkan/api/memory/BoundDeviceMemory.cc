#include "BoundDeviceMemory.h"

BoundDeviceMemory::BoundDeviceMemory(const DeviceMemory* const memory,
                                     ReservedBlock reservedBlock)
    : memory_(memory),
      reservedBlock_(std::move(reservedBlock)),
      allocatedMemoryOffset_(reservedBlock.GetMemoryBlock().offset) {}

void BoundDeviceMemory::Bind(const Buffer& buffer) const {
  memory_->Bind(buffer, allocatedMemoryOffset_);
}

void BoundDeviceMemory::Bind(const Image& image) const {
  memory_->Bind(image, allocatedMemoryOffset_);
}

void* BoundDeviceMemory::Map(const VkDeviceSize offset,
                             const VkDeviceSize size) const {
  return memory_->Map(allocatedMemoryOffset_ + offset, size);
}

void BoundDeviceMemory::Unmap() const {
  memory_->Unmap();
}

void BoundDeviceMemory::MapCopy(const void* const data,
                                const VkDeviceSize size) const {
  memory_->MapCopy(data, allocatedMemoryOffset_, size);
}

void BoundDeviceMemory::FlushMappedMemoryRange(const VkDeviceSize offset,
                                               const VkDeviceSize size) const {
  memory_->FlushMappedMemoryRange(allocatedMemoryOffset_ + offset, size);
}
