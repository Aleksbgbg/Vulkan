#ifndef VULKAN_SRC_DYNAMICUNIFORMBUFFERINITIALIZER_H
#define VULKAN_SRC_DYNAMICUNIFORMBUFFERINITIALIZER_H

#include "game/rendering/BufferWithMemory.h"
#include "memory/Alignment.h"
#include "memory/DeviceMemoryAllocator.h"
#include "util/types.h"
#include "vulkan/DescriptorSet.h"
#include "vulkan/structures/default.h"

class DynamicUniformBufferInitializer {
 public:
  DynamicUniformBufferInitializer(const u32 objectCount,
                                  const VkPhysicalDeviceLimits& limits,
                                  const VirtualDevice& virtualDevice,
                                  DeviceMemoryAllocator& allocator);

  u32 PaddedSize(const u32 elementSize) const;
  BufferWithMemory CreateBuffer(const u32 elementPaddedSize) const;

 private:
  const u32 objectCount;
  const VkPhysicalDeviceLimits& limits;
  const VirtualDevice& virtualDevice;
  DeviceMemoryAllocator& allocator;
};

#endif  // VULKAN_SRC_DYNAMICUNIFORMBUFFERINITIALIZER_H
