#ifndef VULKAN_SRC_GAME_RENDERING_MESHES_MESH_H_
#define VULKAN_SRC_GAME_RENDERING_MESHES_MESH_H_

#include "game/rendering/resources/ResourceBinder.h"
#include "util/include/glm.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/PipelineLayout.h"

class Mesh {
 public:
  virtual ~Mesh() = default;

  virtual void BindTexture(ResourceBinder& textureRegistry) const = 0;

  virtual void Render(const CommandBuffer& commandBuffer,
                      const PipelineLayout& pipelineLayout) const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERING_MESHES_MESH_H_
