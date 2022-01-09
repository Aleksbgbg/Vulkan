#include "BoundBuffer.h"

BoundBuffer::BoundBuffer(vk::Buffer buffer, vk::BoundDeviceMemory memory)
    : buffer_(std::move(buffer)), memory_(std::move(memory)) {}

const vk::Buffer& BoundBuffer::RawBuffer() const {
  return buffer_;
}

vk::DescriptorSet::WriteDescriptorSet BoundBuffer::WriteBuffer(
    const vk::DescriptorSet& descriptorSet,
    const VkDescriptorType descriptorType, const u32 binding) const {
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
