#include "DeviceMemory.h"

#include <cstring>

#include "error.h"
#include "renderer/vulkan/api/structures/MappedMemoryRange.h"

DeviceMemory::DeviceMemory() : memory(nullptr) {}

DeviceMemory::DeviceMemory(VkDevice device,
                           const MemoryAllocateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(
      vkAllocateMemory(device, infoBuilder.Build(), nullptr, &memory));
}

DeviceMemory::DeviceMemory(DeviceMemory&& other) noexcept
    : device(other.device), memory(other.memory) {
  other.memory = nullptr;
}

DeviceMemory::~DeviceMemory() {
  if (memory != nullptr) {
    vkFreeMemory(device, memory, nullptr);
  }
}

DeviceMemory& DeviceMemory::operator=(DeviceMemory&& other) noexcept {
  std::swap(device, other.device);
  std::swap(memory, other.memory);
  return *this;
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
