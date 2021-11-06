#ifndef VULKAN_SRC_GAME_SPACESHIPMESH_H
#define VULKAN_SRC_GAME_SPACESHIPMESH_H

#include <vector>

#include "game/rendering/BufferWithMemory.h"
#include "game/rendering/ImageWithMemory.h"
#include "game/rendering/Texture.h"
#include "game/rendering/meshes/Mesh.h"
#include "game/rendering/resources/TextureRegistry.h"
#include "util/include/glm.h"
#include "util/types.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/PipelineLayout.h"

struct MeshFrame {
  u32 indexCount;
};

class SpaceshipMesh : public Mesh {
 public:
  SpaceshipMesh() = default;
  SpaceshipMesh(BufferWithMemory vertexBuffer, BufferWithMemory indexBuffer,
                Texture texture, Texture emissive,
                std::vector<MeshFrame> meshFrames, glm::vec3 modelCenter,
                glm::vec3 modelSize);

  void WriteTexture(TextureRegistry& textureRegistry) const override;
  void Render(const CommandBuffer& commandBuffer,
              const PipelineLayout& pipelineLayout,
              const glm::mat4& transform) const override;

  glm::vec3 Size() const;
  void LoadFrame(const u32 frame);

 private:
  BufferWithMemory vertexBuffer;
  BufferWithMemory indexBuffer;

  Texture texture;
  Texture emissive;

  std::vector<MeshFrame> meshFrames;

  glm::vec3 modelCenter;
  glm::vec3 modelSize;

  u32 selectedFrame;
};

#endif  // VULKAN_SRC_GAME_SPACESHIPMESH_H
