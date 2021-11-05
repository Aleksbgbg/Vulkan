#ifndef VULKAN_SRC_GAME_RESOURCEALLOCATOR_H
#define VULKAN_SRC_GAME_RESOURCEALLOCATOR_H

#include "game/rendering/resources/ResourceLoader.h"
#include "memory/DeviceMemoryAllocator.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/Fence.h"
#include "vulkan/VirtualDevice.h"

class ResourceAllocator : public ResourceLoader {
 public:
  ResourceAllocator() = default;
  ResourceAllocator(const VirtualDevice& virtualDevice,
                    DeviceMemoryAllocator& deviceAllocator,
                    const CommandBuffer& commandBuffer, const Fence& fence);

  Texture LoadTexture(const std::string_view filename) const override;

  BufferWithMemory AllocateDeviceBuffer(
      const void* const data, const std::size_t size,
      const VkBufferUsageFlags usage) const override;

 private:
  const VirtualDevice* virtualDevice;
  DeviceMemoryAllocator* deviceAllocator;
  const CommandBuffer* commandBuffer;
  const Fence* fence;
};

#endif  // VULKAN_SRC_GAME_RESOURCEALLOCATOR_H
