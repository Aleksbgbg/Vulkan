#include "SpaceshipMesh.h"

SpaceshipMesh::SpaceshipMesh(BufferWithMemory vertexBuffer,
                             BufferWithMemory indexBuffer, Texture texture,
                             Texture emissive,
                             std::vector<MeshFrame> meshFrames,
                             glm::vec3 modelCenter, glm::vec3 modelSize)
    : vertexBuffer(std::move(vertexBuffer)),
      indexBuffer(std::move(indexBuffer)),
      texture(std::move(texture)),
      emissive(std::move(emissive)),
      meshFrames(std::move(meshFrames)),
      modelCenter(modelCenter),
      modelSize(modelSize) {}

void SpaceshipMesh::BindTexture(ResourceBinder& textureRegistry) const {
  textureRegistry.BindTexture(texture.view, 0);
  textureRegistry.BindTexture(emissive.view, 1);
}

void SpaceshipMesh::Render(const CommandBuffer& commandBuffer,
                           const PipelineLayout& pipelineLayout) const {
  commandBuffer.CmdBindVertexBuffers(vertexBuffer.buffer, 0);
  commandBuffer.CmdBindIndexBuffer(indexBuffer.buffer, VK_INDEX_TYPE_UINT16);
  commandBuffer.CmdDrawIndexed(meshFrames[selectedFrame].indexCount);
}

glm::vec3 SpaceshipMesh::Size() const {
  return modelSize;
}

void SpaceshipMesh::LoadFrame(const u32 frame) {
  selectedFrame = std::min(frame, static_cast<u32>(meshFrames.size() - 1));
}
