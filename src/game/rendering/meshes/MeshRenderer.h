#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_MESHRENDERER_H
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_MESHRENDERER_H

#include "Mesh.h"
#include "util/include/glm.h"
#include "vulkan/CommandBuffer.h"

class MeshRenderer {
 public:
  MeshRenderer() = default;
  MeshRenderer(const CommandBuffer& commandBuffer,
               const PipelineLayout& pipelineLayout);

  void Render(const Mesh& mesh) const;
  void RenderTransformed(const Mesh& mesh, const glm::mat4& transform) const;

 private:
  const CommandBuffer* commandBuffer;
  const PipelineLayout* pipelineLayout;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_MESHRENDERER_H
