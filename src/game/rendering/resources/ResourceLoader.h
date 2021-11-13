#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCELOADER_H
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCELOADER_H

#include <string_view>

#include "game/rendering/BufferWithMemory.h"
#include "game/rendering/Texture.h"

class ResourceLoader {
 public:
  virtual ~ResourceLoader() = default;

  virtual Texture LoadTexture(const std::string_view filename) const = 0;

  template <typename TData>
  BufferWithMemory AllocateVertexBuffer(const TData& data) const {
    return AllocateDeviceBuffer(data, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
  }

  template <typename TData>
  BufferWithMemory AllocateIndexBuffer(const TData& data) const {
    return AllocateDeviceBuffer(data, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
  }

  template <typename TData>
  BufferWithMemory AllocateDeviceBuffer(const TData& data,
                                        const VkBufferUsageFlags usage) const {
    return AllocateDeviceBuffer(data.data(), sizeof(data[0]) * data.size(),
                                usage);
  }

  virtual BufferWithMemory AllocateLocalBuffer(
      const std::size_t size, const VkBufferUsageFlags usage) const = 0;
  virtual BufferWithMemory AllocateDeviceBuffer(
      const void* const data, const std::size_t size,
      const VkBufferUsageFlags usage) const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCELOADER_H
