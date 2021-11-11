#ifndef VULKAN_SRC_SCENEDESCRIPTOR_H
#define VULKAN_SRC_SCENEDESCRIPTOR_H

#include "DynamicUniformBufferInitializer.h"
#include "game/rendering/BufferWithMemory.h"
#include "game/rendering/resources/ResourceLoader.h"
#include "game/renders/SceneDescriptorBinder.h"

class SceneDescriptor : public SceneDescriptorBinder {
 public:
  SceneDescriptor() = default;
  SceneDescriptor(
      const VirtualDevice& virtualDevice, const DescriptorPool& descriptorPool,
      DescriptorSetLayout layout, const ResourceLoader& resourceLoader,
      const u32& bufferObjectIndex,
      const DynamicUniformBufferInitializer& viewTransformBufferInitializer);

  static void ConfigureDescriptorPoolSizes(
      std::vector<VkDescriptorPoolSize>& poolSizes);
  static DescriptorSetLayout CreateLayout(const VirtualDevice& virtualDevice);

  void WriteDescriptorSets(

      std::vector<DescriptorSet::WriteDescriptorSet>& descriptorSetWrites)
      const;

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
