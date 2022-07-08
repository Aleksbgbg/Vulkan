#ifndef SRC_RENDERER_VULKAN_API_MEMORY_BOUNDDEVICEMEMORY_H_
#define SRC_RENDERER_VULKAN_API_MEMORY_BOUNDDEVICEMEMORY_H_

#include "memory/ReservedBlock.h"
#include "renderer/vulkan/api/DeviceMemory.h"

namespace vk {

class BoundDeviceMemory {
  friend class DeviceMemoryAllocator;

 public:
  BoundDeviceMemory() = default;

  void* Map(const VkDeviceSize offset, const VkDeviceSize size) const;
  void Unmap() const;

  void MapCopy(const void* const data, const VkDeviceSize size) const;

  void FlushMappedMemoryRange(const VkDeviceSize offset,
                              const VkDeviceSize size) const;

 private:
  BoundDeviceMemory(const DeviceMemory* const memory,
                    ReservedBlock reservedBlock);

  void Bind(const Buffer& buffer) const;
  void Bind(const Image& image) const;

 private:
  const DeviceMemory* memory_;
  ReservedBlock reservedBlock_;

  VkDeviceSize allocatedMemoryOffset_;
};

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_MEMORY_BOUNDDEVICEMEMORY_H_
