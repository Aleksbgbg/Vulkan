#ifndef VULKAN_SRC_VULKAN_BUFFER_H
#define VULKAN_SRC_VULKAN_BUFFER_H

#include <vulkan/vulkan.h>

#include "structures/BufferCreateInfo.h"

class Buffer {
  friend class CommandBuffer;
  friend class DescriptorSet;
  friend class DeviceMemory;

 public:
  Buffer();
  Buffer(VkDevice device, const BufferCreateInfoBuilder& infoBuilder);

  Buffer(const Buffer&) = delete;
  Buffer(Buffer&& other) noexcept;

  ~Buffer();

  Buffer& operator=(const Buffer&) = delete;
  Buffer& operator=(Buffer&& other) noexcept;

  VkDeviceSize Size() const;

  VkMemoryRequirements GetMemoryRequirements() const;

 private:
  VkDevice device;
  VkBuffer buffer;
  VkDeviceSize size;
};

#endif  // VULKAN_SRC_VULKAN_BUFFER_H
