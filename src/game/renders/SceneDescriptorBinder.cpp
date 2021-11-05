#include "SceneDescriptorBinder.h"

// SceneDescriptorBinder::SceneDescriptorBinder(
//     const DynamicUniformBuffer<PerFrameData>& viewTransformBuffer,
//     const DescriptorSet& sceneDescriptorSet, const u32 bufferObjectIndex)
//     : viewTransformBuffer(viewTransformBuffer),
//       sceneDescriptorSet(sceneDescriptorSet),
//       bufferObjectIndex(bufferObjectIndex) {}

void SceneDescriptorBinder::BindActorDescriptorSet(
    const CommandBuffer& commandBuffer,
    const PipelineLayout& pipelineLayout) const {
  //  commandBuffer.CmdBindDescriptorSet(
  //      VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
  //      sceneDescriptorSet,
  //      viewTransformBuffer.CalculateDynamicOffset(bufferObjectIndex));
}
