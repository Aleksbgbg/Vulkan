#ifndef VULKAN_SRC_RENDERER_VULKAN_API_MEMORY_DEVICEMEMORYSUBALLOCATION_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_MEMORY_DEVICEMEMORYSUBALLOCATION_H_

#include "memory/ReservedBlock.h"
#include "renderer/vulkan/api/DeviceMemory.h"

class DeviceMemorySubAllocation {
  friend class DeviceMemoryAllocator;

 public:
  DeviceMemorySubAllocation() = default;

  void* Map(const VkDeviceSize offset, const VkDeviceSize size) const;
  void Unmap() const;

  void MapCopy(const void* const data, const VkDeviceSize size) const;

  void FlushMappedMemoryRange(const VkDeviceSize offset,
                              const VkDeviceSize size) const;

 private:
  DeviceMemorySubAllocation(const DeviceMemory* const memory,
                            ReservedBlock reservedBlock);

  void Bind(const Buffer& buffer) const;
  void Bind(const Image& image) const;

 private:
  const DeviceMemory* memory_;
  ReservedBlock reservedBlock_;

  VkDeviceSize allocatedMemoryOffset_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_MEMORY_DEVICEMEMORYSUBALLOCATION_H_
