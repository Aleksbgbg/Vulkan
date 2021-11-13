#include "DeviceMemory.h"

#include <cstring>

#include "error.h"
#include "vulkan/structures/MappedMemoryRange.h"

DeviceMemory::DeviceMemory(VkDevice device,
                           MemoryAllocateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(
      vkAllocateMemory(device, infoBuilder.Build(), nullptr, &memory));
}

DeviceMemory::~DeviceMemory() {
  if (memory != nullptr) {
    vkFreeMemory(device, memory, nullptr);
  }
}

void* DeviceMemory::Map(const VkDeviceSize offset,
                        const VkDeviceSize size) const {
  void* mappedMemory;
  PROCEED_ON_VALID_RESULT(
      vkMapMemory(device, memory, offset, size, 0, &mappedMemory));
  return mappedMemory;
}

void DeviceMemory::Unmap() const {
  vkUnmapMemory(device, memory);
}

void DeviceMemory::MapCopy(const void* const data, const VkDeviceSize offset,
                           const VkDeviceSize size) const {
  void* mappedMemory;
  PROCEED_ON_VALID_RESULT(
      vkMapMemory(device, memory, offset, size, 0, &mappedMemory));
  std::memcpy(mappedMemory, data, size);
  vkUnmapMemory(device, memory);
}

std::optional<u32> DeviceMemory::FindSuitableMemoryTypeIndex(
    const VkPhysicalDeviceMemoryProperties& memoryProperties,
    const VkMemoryPropertyFlags requiredProperties, const u32 memoryTypeBits) {
  for (u32 index = 0; index < memoryProperties.memoryTypeCount; ++index) {
    const VkMemoryType memoryType = memoryProperties.memoryTypes[index];

    const bool isValidMemoryType = memoryTypeBits & (1 << index);
    const bool hasRequiredProperties =
        (memoryType.propertyFlags & requiredProperties) == requiredProperties;

    if (isValidMemoryType && hasRequiredProperties) {
      return index;
    }
  }

  return std::nullopt;
}

void DeviceMemory::Bind(const Buffer& buffer, const VkDeviceSize offset) const {
  PROCEED_ON_VALID_RESULT(
      vkBindBufferMemory(device, buffer.buffer, memory, offset));
}

void DeviceMemory::Bind(const Image& image, const VkDeviceSize offset) const {
  PROCEED_ON_VALID_RESULT(
      vkBindImageMemory(device, image.image, memory, offset));
}

void DeviceMemory::FlushMappedMemoryRange(const VkDeviceSize offset,
                                          const VkDeviceSize size) const {
  PROCEED_ON_VALID_RESULT(vkFlushMappedMemoryRanges(device, 1,
                                                    MappedMemoryRangeBuilder()
                                                        .SetMemory(memory)
                                                        .SetOffset(offset)
                                                        .SetSize(size)
                                                        .Build()));
}
