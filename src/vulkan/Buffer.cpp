#include <stdexcept>

#include "Buffer.h"
#include "CommandBuffer.h"

#include "error.h"

Buffer::Buffer(
    VkDevice device,
    VkPhysicalDeviceMemoryProperties* memoryProperties,
    BufferCreateInfoBuilder& infoBuilder)
    : device(device), memoryProperties(memoryProperties) {
  VkBufferCreateInfo* bufferCreateInfo = infoBuilder.Build();
  size = bufferCreateInfo->size;
  PROCEED_ON_VALID_RESULT(vkCreateBuffer(device, bufferCreateInfo, nullptr, &buffer))
}

Buffer::~Buffer() {
  if (buffer != nullptr) {
    vkDestroyBuffer(device, buffer, nullptr);
  }
}

DeviceMemory Buffer::AllocateAndBindMemory(const VkMemoryPropertyFlags requiredProperties) const {
  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

  std::optional<u32> memoryTypeIndex =
      DeviceMemory::FindSuitableMemoryTypeIndex(
          *memoryProperties, requiredProperties, memoryRequirements.memoryTypeBits);

  if (!memoryTypeIndex.has_value()) {
    throw std::runtime_error("Could not find suitable GPU memory to allocate.");
  }

  return
      std::move(
          DeviceMemory(
              device,
              MemoryAllocateInfoBuilder()
                  .SetAllocationSize(memoryRequirements.size)
                  .SetMemoryTypeIndex(memoryTypeIndex.value()))
              .BindAll(buffer));
}

VkDeviceSize Buffer::Size() const {
  return size;
}
