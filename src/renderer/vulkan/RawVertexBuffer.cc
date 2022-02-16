#include "RawVertexBuffer.h"

RawVertexBuffer::RawVertexBuffer(BoundBuffer vertexBuffer)
    : vertexBuffer_(std::move(vertexBuffer)) {}

void RawVertexBuffer::Draw(const vk::CommandBuffer& commandBuffer,
                           const u32 instances) const {
  commandBuffer.CmdBindVertexBuffers(vertexBuffer_.RawBuffer());
  commandBuffer.CmdDraw(0, instances);
}
