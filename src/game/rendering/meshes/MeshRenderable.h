#ifndef VULKAN_SRC_GAME_RENDERING_MESHES_MESHRENDERABLE_H_
#define VULKAN_SRC_GAME_RENDERING_MESHES_MESHRENDERABLE_H_

#include "vulkan/CommandBuffer.h"
#include "vulkan/PipelineLayout.h"

class MeshRenderable {
 public:
  virtual ~MeshRenderable() = default;

  virtual void Render(const CommandBuffer& commandBuffer,
                      const PipelineLayout& pipelineLayout) const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERING_MESHES_MESHRENDERABLE_H_
