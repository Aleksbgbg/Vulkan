#include "DeviceMemory.h"

#include "error.h"

DeviceMemory::DeviceMemory(VkDevice device, MemoryAllocateInfoBuilder& infoBuilder) : device(device) {
  PROCEED_ON_VALID_RESULT(vkAllocateMemory(device, infoBuilder.Build(), nullptr, &memory))
}

DeviceMemory::~DeviceMemory() {
  if (memory != nullptr) {
    vkFreeMemory(device, memory, nullptr);
  }
}

DeviceMemory& DeviceMemory::BindAll(VkBuffer buffer) {
  return Bind(buffer, 0);
}

DeviceMemory& DeviceMemory::Bind(VkBuffer buffer, const VkDeviceSize offset) {
  PROCEED_ON_VALID_RESULT(vkBindBufferMemory(device, buffer, memory, offset))
  return *this;
}

DeviceMemory& DeviceMemory::BindAll(VkImage image) {
  return Bind(image, 0);
}

DeviceMemory& DeviceMemory::Bind(VkImage image, const VkDeviceSize offset) {
  PROCEED_ON_VALID_RESULT(vkBindImageMemory(device, image, memory, offset))
  return *this;
}

void DeviceMemory::MapCopy(const void* data, const u64 size) {
  void* mappedMemory;
  PROCEED_ON_VALID_RESULT(vkMapMemory(device, memory, 0, size, 0, &mappedMemory))
  memcpy(mappedMemory, data, size);
  vkUnmapMemory(device, memory);
}

std::optional<u32> DeviceMemory::FindSuitableMemoryTypeIndex(
    const VkPhysicalDeviceMemoryProperties& memoryProperties,
    const VkMemoryPropertyFlags requiredProperties,
    const u32 memoryTypeBits) {
  for (u32 index = 0; index < memoryProperties.memoryTypeCount; ++index) {
    const VkMemoryType memoryType = memoryProperties.memoryTypes[index];

    const bool isValidMemoryType = memoryTypeBits & (1 << index);
    const bool hasRequiredProperties = (memoryType.propertyFlags & requiredProperties) == requiredProperties;

    if (isValidMemoryType && hasRequiredProperties) {
      return index;
    }
  }

  return std::nullopt;
}
