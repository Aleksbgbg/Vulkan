#ifndef VULKAN_SRC_RENDERER_VULKAN_RAWVERTEXBUFFER_H_
#define VULKAN_SRC_RENDERER_VULKAN_RAWVERTEXBUFFER_H_

#include "BoundBuffer.h"
#include "VertexBuffer.h"

class RawVertexBuffer : public VertexBuffer {
 public:
  RawVertexBuffer(BoundBuffer vertexBuffer);

  void Draw(const vk::CommandBuffer& commandBuffer,
            u32 instances) const override;

 private:
  BoundBuffer vertexBuffer_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RAWVERTEXBUFFER_H_
