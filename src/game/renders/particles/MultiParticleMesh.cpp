#include "MultiParticleMesh.h"

MultiParticleMesh::MultiParticleMesh(BufferWithMemory vertexBuffer,
                                     BufferWithMemory indexBuffer,
                                     const u32 indexCount)
    : vertexBuffer_(std::move(vertexBuffer)),
      indexBuffer_(std::move(indexBuffer)),
      indexCount_(indexCount) {}

MultiParticleMesh::SubMesh MultiParticleMesh::RenderInstances(
    const u32 instanceCount) const {
  return SubMesh(vertexBuffer_, indexBuffer_, indexCount_, instanceCount);
}

MultiParticleMesh::SubMesh::SubMesh(const BufferWithMemory& vertexBuffer,
                                    const BufferWithMemory& indexBuffer,
                                    const u32 indexCount,
                                    const u32 instanceCount)
    : vertexBuffer_(vertexBuffer),
      indexBuffer_(indexBuffer),
      indexCount_(indexCount),
      instanceCount_(instanceCount) {}

void MultiParticleMesh::SubMesh::BindTexture(
    ResourceBinder& textureRegistry) const {}

void MultiParticleMesh::SubMesh::Render(
    const CommandBuffer& commandBuffer,
    const PipelineLayout& pipelineLayout) const {
  if (instanceCount_ == 0) {
    return;
  }

  commandBuffer.CmdBindVertexBuffers(vertexBuffer_.buffer, 0);
  commandBuffer.CmdBindIndexBuffer(indexBuffer_.buffer, VK_INDEX_TYPE_UINT16);
  commandBuffer.CmdDrawIndexedInstanced(indexCount_, instanceCount_);
}
