#ifndef VULKAN_SRC_SCENEDESCRIPTOR_H
#define VULKAN_SRC_SCENEDESCRIPTOR_H

#include "BufferWithMemory.h"
#include "game/rendering/resources/ResourceLoader.h"
#include "game/renders/DescriptorSetLayoutFactory.h"
#include "game/renders/SceneDescriptorBinder.h"
#include "vulkan/VirtualDevice.h"

class SceneDescriptor : public SceneDescriptorBinder {
 public:
  static DescriptorSetLayout CreateSceneDescriptorLayout(
      const DescriptorSetLayoutFactory& descriptorSetLayoutFactory);

  SceneDescriptor() = default;
  SceneDescriptor(
      DescriptorSetLayout layout, const DescriptorPool& descriptorPool,
      DynamicUniformBufferInitializer& dynamicUniformBufferInitializer,
      const u32& bufferObjectIndex);

  DescriptorSet::WriteDescriptorSet WriteDescriptorSet() const;

  PerFrameData& FrameData();
  void FlushFrameData() const;

  void BindActorDescriptorSet(
      const CommandBuffer& commandBuffer,
      const PipelineLayout& pipelineLayout) const override;

  void UpdateAspect(const float aspect);

 private:
  DescriptorSetLayout sceneDescriptorSetLayout;
  DescriptorSet sceneDescriptorSet;
  DynamicUniformBuffer<PerFrameData> viewTransformBuffer;
  const u32* bufferObjectIndex;
};

#endif  // VULKAN_SRC_SCENEDESCRIPTOR_H
