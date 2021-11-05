#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(const CommandBuffer& commandBuffer,
                           const PipelineLayout& pipelineLayout)
    : commandBuffer(&commandBuffer), pipelineLayout(&pipelineLayout) {}

void MeshRenderer::Render(const Mesh& mesh, const glm::mat4& transform) const {
  mesh.Render(*commandBuffer, *pipelineLayout, transform);
}
