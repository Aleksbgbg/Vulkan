#include "LightMesh.h"

LightMesh::LightMesh(BufferWithMemory vertexBuffer,
                     BufferWithMemory indexBuffer, const u32 indexCount)
    : vertexBuffer(std::move(vertexBuffer)),
      indexBuffer(std::move(indexBuffer)),
      indexCount(indexCount) {}

void LightMesh::WriteTexture(TextureRegistry& textureRegistry) const {}

void LightMesh::Render(const CommandBuffer& commandBuffer,
                       const PipelineLayout& pipelineLayout,
                       const glm::mat4& transform) const {
  commandBuffer.CmdBindVertexBuffers(vertexBuffer.buffer, 0);
  commandBuffer.CmdBindIndexBuffer(indexBuffer.buffer, VK_INDEX_TYPE_UINT16);
  commandBuffer.CmdDrawIndexed(indexCount,
                               /* instanceCount= */ 1);
}
