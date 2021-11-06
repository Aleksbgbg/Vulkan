#include "TexturedMesh.h"

TexturedMesh::TexturedMesh(BufferWithMemory vertexBuffer,
                       BufferWithMemory indexBuffer, const u32 indexCount,
                       Texture texture)
    : vertexBuffer(std::move(vertexBuffer)),
      indexBuffer(std::move(indexBuffer)),
      indexCount(indexCount),
      texture(std::move(texture)) {}

void TexturedMesh::WriteTexture(TextureRegistry& textureRegistry) const {
  textureRegistry.WriteTexture(texture.view, 0);
}

void TexturedMesh::Render(const CommandBuffer& commandBuffer,
                        const PipelineLayout& pipelineLayout,
                        const glm::mat4& transform) const {
  commandBuffer.CmdBindVertexBuffers(vertexBuffer.buffer, 0);
  commandBuffer.CmdBindIndexBuffer(indexBuffer.buffer, VK_INDEX_TYPE_UINT16);
  commandBuffer.CmdDrawIndexed(indexCount,
                               /* instanceCount= */ 1);
}
