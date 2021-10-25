#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_MESHLOADER_H
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_MESHLOADER_H

#include <string_view>
#include <vector>

#include "Mesh.h"
#include "memory/DeviceMemoryAllocator.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/Fence.h"

struct MeshFrameLoadParams {
  std::string_view model;
};

struct MeshLoadParams {
  std::vector<MeshFrameLoadParams> frames;

  std::string_view texture;
  std::string_view emissive;
};

class MeshLoader {
 public:
  MeshLoader(const VirtualDevice& virtualDevice,
             DeviceMemoryAllocator& deviceAllocator,
             const CommandBuffer& commandBuffer, const Fence& fence);

  Mesh LoadMesh(const MeshLoadParams& params) const;

 private:
  Texture LoadTexture(const std::string_view filename) const;
  BufferWithMemory AllocateDeviceBuffer(const void* const data,
                                        const std::size_t size,
                                        const VkBufferUsageFlags usage) const;

 private:
  const VirtualDevice& virtualDevice;
  DeviceMemoryAllocator& deviceAllocator;
  const CommandBuffer& commandBuffer;
  const Fence& fence;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_MESHLOADER_H
