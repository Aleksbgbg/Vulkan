#ifndef VULKAN_SRC_VULKAN_BUFFER_H
#define VULKAN_SRC_VULKAN_BUFFER_H

#include <optional>

#include "lifetime_semantics.h"
#include "structures/BufferCreateInfo.h"
#include "DeviceMemory.h"

class CommandBuffer;

class Buffer {
  friend class CommandBuffer;
  friend class DescriptorSet;
public:
  Buffer() = default;
  explicit Buffer(VkDevice device,
                  VkPhysicalDeviceMemoryProperties* memoryProperties,
                  BufferCreateInfoBuilder& infoBuilder);

  Buffer(const Buffer&) = delete;
  Buffer(Buffer&& other) noexcept
    : device(other.device), memoryProperties(other.memoryProperties), buffer(other.buffer), size(other.size) {
    other.buffer = nullptr;
  }

  ~Buffer();

  Buffer& operator=(const Buffer&) = delete;
  Buffer& operator=(Buffer&& other) noexcept {
    device = other.device;
    memoryProperties = other.memoryProperties;
    std::swap(buffer, other.buffer);
    size = other.size;
    return *this;
  }

  DeviceMemory AllocateAndBindMemory(const VkMemoryPropertyFlags requiredProperties) const;

  VkDeviceSize Size() const;

private:
  VkDevice device;
  VkPhysicalDeviceMemoryProperties* memoryProperties;
  VkBuffer buffer = nullptr;
  VkDeviceSize size;
};

#endif // VULKAN_SRC_VULKAN_BUFFER_H
