#include "Mesh.h"

Mesh::Mesh(BufferWithMemory vertexBuffer, BufferWithMemory indexBuffer,
           ImageWithMemory texture, ImageView textureView,
           std::vector<MeshFrame> meshFrames, glm::vec3 modelCenter,
           glm::vec3 modelSize)
    : vertexBuffer(std::move(vertexBuffer)),
      indexBuffer(std::move(indexBuffer)),
      texture(std::move(texture)),
      textureView(std::move(textureView)),
      meshFrames(std::move(meshFrames)),
      modelCenter(modelCenter),
      modelSize(modelSize) {}

glm::vec3 Mesh::Size() const {
  return modelSize;
}

void Mesh::WriteTexture(TextureRegistry& textureRegistry) const {
  textureRegistry.WriteTexture(textureView);
}

void Mesh::LoadFrame(const u32 frame) {
  selectedFrame = frame;
}
