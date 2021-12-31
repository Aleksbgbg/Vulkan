#include "Buffer.h"

#include "CommandBuffer.h"
#include "DeviceMemory.h"
#include "error.h"

Buffer::Buffer() : buffer_(nullptr) {}

Buffer::Buffer(VkDevice device, const BufferCreateInfoBuilder& infoBuilder)
    : device_(device) {
  const VkBufferCreateInfo* const bufferCreateInfo = infoBuilder.Build();
  size_ = bufferCreateInfo->size;
  PROCEED_ON_VALID_RESULT(
      vkCreateBuffer(device, bufferCreateInfo, nullptr, &buffer_));
}

Buffer::Buffer(Buffer&& other) noexcept
    : device_(other.device_), buffer_(other.buffer_), size_(other.size_) {
  other.buffer_ = nullptr;
}

Buffer::~Buffer() {
  if (buffer_ != nullptr) {
    vkDestroyBuffer(device_, buffer_, nullptr);
  }
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
  std::swap(device_, other.device_);
  std::swap(buffer_, other.buffer_);
  size_ = other.size_;
  return *this;
}

VkDeviceSize Buffer::Size() const {
  return size_;
}

VkMemoryRequirements Buffer::GetMemoryRequirements() const {
  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device_, buffer_, &memoryRequirements);
  return memoryRequirements;
}
