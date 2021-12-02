#ifndef VULKAN_SRC_GAME_RENDERS_SCENEDESCRIPTORBINDER_H_
#define VULKAN_SRC_GAME_RENDERS_SCENEDESCRIPTORBINDER_H_

#include "DynamicUniformBuffer.h"
#include "ViewTransform.h"
#include "util/types.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/PipelineLayout.h"

class SceneDescriptorBinder {
 public:
  //  SceneDescriptorBinder(
  //      const DynamicUniformBuffer<PerFrameData>& viewTransformBuffer,
  //      const DescriptorSet& sceneDescriptorSet, const u32 bufferObjectIndex);
  virtual void BindActorDescriptorSet(
      const CommandBuffer& commandBuffer,
      const PipelineLayout& pipelineLayout) const;

 private:
  //  const DynamicUniformBuffer<PerFrameData>& viewTransformBuffer;
  //  const DescriptorSet& sceneDescriptorSet;
  //  const u32 bufferObjectIndex;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SCENEDESCRIPTORBINDER_H_
