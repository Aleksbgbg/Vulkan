#include "Buffer.h"

#include <stdexcept>

#include "CommandBuffer.h"
#include "DeviceMemory.h"
#include "error.h"

Buffer::Buffer() : buffer(nullptr) {}

Buffer::Buffer(VkDevice device, const BufferCreateInfoBuilder& infoBuilder)
    : device(device) {
  const VkBufferCreateInfo* bufferCreateInfo = infoBuilder.Build();
  size = bufferCreateInfo->size;
  PROCEED_ON_VALID_RESULT(
      vkCreateBuffer(device, bufferCreateInfo, nullptr, &buffer));
}

Buffer::Buffer(Buffer&& other) noexcept
    : device(other.device), buffer(other.buffer), size(other.size) {
  other.buffer = nullptr;
}

Buffer::~Buffer() {
  if (buffer != nullptr) {
    vkDestroyBuffer(device, buffer, nullptr);
  }
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
  std::swap(device, other.device);
  std::swap(buffer, other.buffer);
  size = other.size;
  return *this;
}

VkDeviceSize Buffer::Size() const {
  return size;
}

VkMemoryRequirements Buffer::GetMemoryRequirements() const {
  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);
  return memoryRequirements;
}
