#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_MESH_H
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_MESH_H

#include "game/rendering/resources/TextureRegistry.h"
#include "util/include/glm.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/PipelineLayout.h"

class Mesh {
 public:
  virtual ~Mesh() = default;

  virtual void WriteTexture(TextureRegistry& textureRegistry) const = 0;

  virtual void Render(const CommandBuffer& commandBuffer,
                      const PipelineLayout& pipelineLayout) const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_MESH_H
