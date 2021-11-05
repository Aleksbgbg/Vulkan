#ifndef VULKAN_SRC_GAME_SKYBOXMESH_H
#define VULKAN_SRC_GAME_SKYBOXMESH_H

#include "game/rendering/BufferWithMemory.h"
#include "game/rendering/Texture.h"
#include "game/rendering/resources/Mesh.h"

class SkyboxMesh : public Mesh {
 public:
  SkyboxMesh() = default;
  SkyboxMesh(BufferWithMemory vertexBuffer, BufferWithMemory indexBuffer,
             const u32 indexCount, Texture texture);

  void WriteTexture(TextureRegistry& textureRegistry) const override;
  void Render(const CommandBuffer& commandBuffer,
              const PipelineLayout& pipelineLayout,
              const glm::mat4& transform) const override;

 private:
  BufferWithMemory vertexBuffer;
  BufferWithMemory indexBuffer;
  u32 indexCount;

  Texture texture;
};

#endif  // VULKAN_SRC_GAME_SKYBOXMESH_H
