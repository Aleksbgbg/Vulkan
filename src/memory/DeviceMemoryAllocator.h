#ifndef VULKAN_SRC_MEMORY_DEVICEMEMORYALLOCATOR_H
#define VULKAN_SRC_MEMORY_DEVICEMEMORYALLOCATOR_H

#include <array>
#include <memory>
#include <vector>

#include "vulkan/Buffer.h"
#include "vulkan/DeviceMemory.h"
#include "vulkan/Image.h"
#include "DeviceHeap.h"

class DeviceMemoryAllocator {
public:
  DeviceMemoryAllocator() = default;
  DeviceMemoryAllocator(VkDevice device, VkPhysicalDeviceMemoryProperties* const memoryProperties);

  ReservedMemory BindMemory(const Buffer& buffer, const VkMemoryPropertyFlags requiredProperties);
  ReservedMemory BindMemory(const Image& image, const VkMemoryPropertyFlags requiredProperties);

private:
  ReservedMemory ReserveMemoryBlock(
      const VkMemoryRequirements& memoryRequirements, const VkMemoryPropertyFlags requiredProperties);

private:
  std::array<DeviceHeap, VK_MAX_MEMORY_TYPES> memoryIndexHeap;

  VkDevice device;
  VkPhysicalDeviceMemoryProperties* memoryProperties;
};

#endif // VULKAN_SRC_MEMORY_DEVICEMEMORYALLOCATOR_H
