#include "Mesh.h"

Mesh::Mesh(BufferWithMemory vertexBuffer, BufferWithMemory indexBuffer,
           Texture texture, Texture emissive, std::vector<MeshFrame> meshFrames,
           glm::vec3 modelCenter, glm::vec3 modelSize)
    : vertexBuffer(std::move(vertexBuffer)),
      indexBuffer(std::move(indexBuffer)),
      texture(std::move(texture)),
      emissive(std::move(emissive)),
      meshFrames(std::move(meshFrames)),
      modelCenter(modelCenter),
      modelSize(modelSize) {}

glm::vec3 Mesh::Size() const {
  return modelSize;
}

void Mesh::WriteTexture(TextureRegistry& textureRegistry) const {
  textureRegistry.WriteTexture(texture.view, 0);
  textureRegistry.WriteTexture(emissive.view, 1);
}

void Mesh::LoadFrame(const u32 frame) {
  selectedFrame = std::min(frame, static_cast<u32>(meshFrames.size() - 1));
}
