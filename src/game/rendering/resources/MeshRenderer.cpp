#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(const CommandBuffer& commandBuffer,
                           const PipelineLayout& pipelineLayout)
    : commandBuffer(&commandBuffer), pipelineLayout(&pipelineLayout) {}

void MeshRenderer::Render(const Mesh& mesh, const glm::mat4& transform) const {
  commandBuffer->CmdBindVertexBuffers(mesh.vertexBuffer.buffer, 0);
  commandBuffer->CmdBindIndexBuffer(mesh.indexBuffer.buffer,
                                    VK_INDEX_TYPE_UINT16);
  commandBuffer->CmdPushConstants(*pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
                                  0, sizeof(transform), &transform);
  commandBuffer->CmdDrawIndexed(mesh.meshFrames[mesh.selectedFrame].indexCount,
                                /* instanceCount= */ 1);
}
