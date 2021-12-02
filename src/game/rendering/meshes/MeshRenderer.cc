#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(const CommandBuffer& commandBuffer,
                           const PipelineLayout& pipelineLayout)
    : commandBuffer(&commandBuffer), pipelineLayout(&pipelineLayout) {}

void MeshRenderer::Render(const Mesh& mesh) const {
  mesh.Render(*commandBuffer, *pipelineLayout);
}

void MeshRenderer::RenderTransformed(const Mesh& mesh,
                                     const glm::mat4& transform) const {
  commandBuffer->CmdPushConstants(*pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
                                  0, sizeof(transform), &transform);
  Render(mesh);
}
