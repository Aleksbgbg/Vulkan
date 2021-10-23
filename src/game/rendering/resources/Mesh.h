#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_MESH_H
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_MESH_H

#include <vector>

#include "game/rendering/BufferWithMemory.h"
#include "game/rendering/ImageWithMemory.h"
#include "game/rendering/resources/TextureRegistry.h"
#include "util/include/glm.h"
#include "util/types.h"

struct MeshFrame {
  u32 indexCount;
};

class Mesh {
  friend class MeshRenderer;

 public:
  Mesh() = default;
  Mesh(BufferWithMemory vertexBuffer, BufferWithMemory indexBuffer,
       ImageWithMemory texture, ImageView textureView,
       std::vector<MeshFrame> meshFrames, glm::vec3 modelCenter,
       glm::vec3 modelSize);

  glm::vec3 Size() const;

  void WriteTexture(TextureRegistry& textureRegistry) const;

  void LoadFrame(const u32 frame);

 private:
  BufferWithMemory vertexBuffer;
  BufferWithMemory indexBuffer;

  ImageWithMemory texture;
  ImageView textureView;

  std::vector<MeshFrame> meshFrames;

  glm::vec3 modelCenter;
  glm::vec3 modelSize;

  u32 selectedFrame;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_MESH_H
