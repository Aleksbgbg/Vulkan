#include "MultiParticleMesh.h"

MultiParticleMesh::MultiParticleMesh(BufferWithMemory vertexBuffer,
                                     BufferWithMemory indexBuffer,
                                     const u32 indexCount,
                                     const u32 instanceCount)
    : vertexBuffer(std::move(vertexBuffer)),
      indexBuffer(std::move(indexBuffer)),
      indexCount(indexCount),
      instanceCount(instanceCount) {}

void MultiParticleMesh::WriteTexture(TextureRegistry& textureRegistry) const {}

void MultiParticleMesh::Render(const CommandBuffer& commandBuffer,
                               const PipelineLayout& pipelineLayout) const {
  commandBuffer.CmdBindVertexBuffers(vertexBuffer.buffer, 0);
  commandBuffer.CmdBindIndexBuffer(indexBuffer.buffer, VK_INDEX_TYPE_UINT16);
  commandBuffer.CmdDrawIndexedInstanced(indexCount, instanceCount);
}
