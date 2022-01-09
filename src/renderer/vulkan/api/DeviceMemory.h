#ifndef VULKAN_SRC_RENDERER_VULKAN_API_DEVICEMEMORY_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_DEVICEMEMORY_H_

#include <vulkan/vulkan.h>

#include <optional>

#include "Buffer.h"
#include "Image.h"
#include "renderer/vulkan/api/structures/MemoryAllocateInfo.h"

namespace vk {

class DeviceMemory {
 public:
  DeviceMemory();
  DeviceMemory(VkDevice device, const MemoryAllocateInfoBuilder& infoBuilder);

  DeviceMemory(const DeviceMemory&) = delete;
  DeviceMemory(DeviceMemory&& other) noexcept;

  ~DeviceMemory();

  DeviceMemory& operator=(const DeviceMemory&) = delete;
  DeviceMemory& operator=(DeviceMemory&& other) noexcept;

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
  VkDeviceMemory memory;
};

}  // namespace vk

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_DEVICEMEMORY_H_
