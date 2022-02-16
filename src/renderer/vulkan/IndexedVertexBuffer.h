#ifndef VULKAN_SRC_RENDERER_VULKAN_INDEXEDVERTEXBUFFER_H_
#define VULKAN_SRC_RENDERER_VULKAN_INDEXEDVERTEXBUFFER_H_

#include "BoundBuffer.h"
#include "VertexBuffer.h"

class IndexedVertexBuffer : public VertexBuffer {
 public:
  IndexedVertexBuffer() = default;
  IndexedVertexBuffer(BoundBuffer vertexIndexBuffer,
                      VkDeviceSize indexDataOffset, u32 indexCount);

  void Draw(const vk::CommandBuffer& commandBuffer,
            u32 instances) const override;

 private:
  BoundBuffer vertexIndexBuffer_;
  VkDeviceSize indexDataOffset_;
  u32 indexCount_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_INDEXEDVERTEXBUFFER_H_
