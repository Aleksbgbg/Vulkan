#ifndef VULKAN_SRC_RENDERER_VULKAN_VERTEXBUFFER_H_
#define VULKAN_SRC_RENDERER_VULKAN_VERTEXBUFFER_H_

#include "core/types.h"
#include "renderer/vulkan/api/CommandBuffer.h"

class VertexBuffer {
 public:
  virtual ~VertexBuffer() = default;

  virtual void Draw(const vk::CommandBuffer& commandBuffer,
                    u32 instances) const = 0;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_VERTEXBUFFER_H_
