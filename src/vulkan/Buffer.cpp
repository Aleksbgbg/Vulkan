#include "Buffer.h"

#include <stdexcept>

#include "CommandBuffer.h"
#include "DeviceMemory.h"
#include "error.h"

Buffer::Buffer(VkDevice device,
               VkPhysicalDeviceMemoryProperties* memoryProperties,
               BufferCreateInfoBuilder& infoBuilder)
    : device(device), memoryProperties(memoryProperties) {
  VkBufferCreateInfo* bufferCreateInfo = infoBuilder.Build();
  size = bufferCreateInfo->size;
  PROCEED_ON_VALID_RESULT(
      vkCreateBuffer(device, bufferCreateInfo, nullptr, &buffer));
}

Buffer::~Buffer() {
  if (buffer != nullptr) {
    vkDestroyBuffer(device, buffer, nullptr);
  }
}

VkDeviceSize Buffer::Size() const {
  return size;
}

VkMemoryRequirements Buffer::GetMemoryRequirements() const {
  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);
  return memoryRequirements;
}
