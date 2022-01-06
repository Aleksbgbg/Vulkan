#include "BoundBuffer.h"

BoundBuffer::BoundBuffer(Buffer buffer, BoundDeviceMemory memory)
    : buffer_(std::move(buffer)), memory_(std::move(memory)) {}

const Buffer& BoundBuffer::RawBuffer() const {
  return buffer_;
}

DescriptorSet::WriteDescriptorSet BoundBuffer::WriteBuffer(
    const DescriptorSet& descriptorSet, const VkDescriptorType descriptorType,
    const u32 binding) const {
  return descriptorSet.CreateBufferWrite(buffer_, descriptorType, binding);
}

void* BoundBuffer::Map() const {
  return memory_.Map(0, buffer_.Size());
}

void BoundBuffer::Unmap() const {
  memory_.Unmap();
}

void BoundBuffer::MapCopy(const void* const data) const {
  MapCopy(data, buffer_.Size());
}

void BoundBuffer::MapCopy(const void* const data,
                          const VkDeviceSize size) const {
  memory_.MapCopy(data, size);
}
