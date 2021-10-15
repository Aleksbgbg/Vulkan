#ifndef VULKAN_SRC_VULKAN_DEVICEMEMORY_H
#define VULKAN_SRC_VULKAN_DEVICEMEMORY_H

#include <vulkan/vulkan_core.h>

#include <optional>

#include "Buffer.h"
#include "Image.h"
#include "lifetime_semantics.h"
#include "structures/MemoryAllocateInfo.h"

class DeviceMemory {
 public:
  VULKAN_OBJECT_MOVABLE_ROOT(DeviceMemory, device, memory)

  DeviceMemory() = default;
  explicit DeviceMemory(VkDevice device,
                        MemoryAllocateInfoBuilder& infoBuilder);

  DeviceMemory(const DeviceMemory&) = delete;

  ~DeviceMemory();

  DeviceMemory& operator=(const DeviceMemory&) = delete;

  void Bind(const Buffer& buffer, const VkDeviceSize offset) const;
  void Bind(const Image& image, const VkDeviceSize offset) const;

  void* Map(const VkDeviceSize offset, const VkDeviceSize size) const;
  void Unmap() const;

  void MapCopy(const void* const data, const VkDeviceSize offset,
               const VkDeviceSize size) const;

  void FlushMappedMemoryRange(const VkDeviceSize offset,
                              const VkDeviceSize size) const;

  static std::optional<u32> FindSuitableMemoryTypeIndex(
      const VkPhysicalDeviceMemoryProperties& memoryProperties,
      const VkMemoryPropertyFlags requiredProperties, const u32 memoryTypeBits);

 private:
  VkDevice device;
  VkDeviceMemory memory = nullptr;
};

#endif  // VULKAN_SRC_VULKAN_DEVICEMEMORY_H
