#ifndef VULKAN_SRC_GAME_LIGHTMESH_H
#define VULKAN_SRC_GAME_LIGHTMESH_H

#include "game/rendering/BufferWithMemory.h"
#include "game/rendering/resources/Mesh.h"

class LightMesh : public Mesh {
 public:
  LightMesh() = default;
  LightMesh(BufferWithMemory vertexBuffer, BufferWithMemory indexBuffer,
            const u32 indexCount);

  void WriteTexture(TextureRegistry& textureRegistry) const override;
  void Render(const CommandBuffer& commandBuffer,
              const PipelineLayout& pipelineLayout,
              const glm::mat4& transform) const override;

 private:
  BufferWithMemory vertexBuffer;
  BufferWithMemory indexBuffer;

  u32 indexCount;
};

#endif  // VULKAN_SRC_GAME_LIGHTMESH_H
