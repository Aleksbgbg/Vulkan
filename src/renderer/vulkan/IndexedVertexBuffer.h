#ifndef VULKAN_SRC_RENDERER_VULKAN_INDEXEDVERTEXBUFFER_H_
#define VULKAN_SRC_RENDERER_VULKAN_INDEXEDVERTEXBUFFER_H_

#include "BoundBuffer.h"
#include "renderer/vulkan/api/CommandBuffer.h"

class IndexedVertexBuffer {
 public:
  IndexedVertexBuffer(BoundBuffer vertexIndexBuffer,
                      VkDeviceSize indexDataOffset, u32 indexCount);

  void DrawInstanced(const CommandBuffer& commandBuffer, u32 instances) const;

 private:
  BoundBuffer vertexIndexBuffer_;
  VkDeviceSize indexDataOffset_;
  u32 indexCount_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_INDEXEDVERTEXBUFFER_H_
