#ifndef VULKAN_SRC_GAME_RENDERING_MESHES_TEXTUREDMESH_H_
#define VULKAN_SRC_GAME_RENDERING_MESHES_TEXTUREDMESH_H_

#include "BufferWithMemory.h"
#include "game/rendering/Texture.h"
#include "game/rendering/meshes/Mesh.h"

class TexturedMesh : public Mesh {
 public:
  TexturedMesh() = default;
  TexturedMesh(BufferWithMemory vertexBuffer, BufferWithMemory indexBuffer,
               const u32 indexCount, Texture texture);

  void BindTexture(ResourceBinder& textureRegistry) const override;
  void Render(const CommandBuffer& commandBuffer,
              const PipelineLayout& pipelineLayout) const override;

 private:
  BufferWithMemory vertexBuffer;
  BufferWithMemory indexBuffer;
  u32 indexCount;

  Texture texture;
};

#endif  // VULKAN_SRC_GAME_RENDERING_MESHES_TEXTUREDMESH_H_
