#include "SceneDescriptor.h"

#include <array>

SceneDescriptor::SceneDescriptor(
    const VirtualDevice& virtualDevice, const DescriptorPool& descriptorPool,
    DescriptorSetLayout layout, const ResourceLoader& resourceLoader,
    const u32& bufferObjectIndex,
    const DynamicUniformBufferInitializer& viewTransformBufferInitializer)
    : sceneDescriptorSetLayout(std::move(layout)),
      sceneDescriptorSet(
          descriptorPool.AllocateDescriptorSet(sceneDescriptorSetLayout)),
      viewTransformBuffer(viewTransformBufferInitializer, sceneDescriptorSet),
      bufferObjectIndex(&bufferObjectIndex) {}

void SceneDescriptor::ConfigureDescriptorPoolSizes(
    std::vector<VkDescriptorPoolSize>& poolSizes) {
  poolSizes.push_back(DescriptorPoolSizeBuilder()
                          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC)
                          .SetDescriptorCount(1));
  poolSizes.push_back(DescriptorPoolSizeBuilder()
                          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                          .SetDescriptorCount(1));
}

DescriptorSetLayout SceneDescriptor::CreateLayout(
    const VirtualDevice& virtualDevice) {
  return virtualDevice.CreateDescriptorSetLayout(
      DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
          DescriptorSetLayoutBindingBuilder()
              .SetBinding(0)
              .SetDescriptorCount(1)
              .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC)
              .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT |
                             VK_SHADER_STAGE_FRAGMENT_BIT)));
}

void SceneDescriptor::WriteDescriptorSets(
    std::vector<std::unique_ptr<DescriptorSet::WriteDescriptorSet>>&
        descriptorSetWrites) const {
  std::unique_ptr<DescriptorSet::WriteDescriptorSet> viewBufferWrite =
      std::make_unique<DescriptorSet::WriteDescriptorSet>();
  viewTransformBuffer.CreateWriteDescriptorSet(0, *viewBufferWrite);
  descriptorSetWrites.emplace_back(std::move(viewBufferWrite));
}

PerFrameData& SceneDescriptor::FrameData() {
  return viewTransformBuffer.Value();
}

void SceneDescriptor::FlushFrameData() const {
  viewTransformBuffer.Flush(*bufferObjectIndex);
}

void SceneDescriptor::BindActorDescriptorSet(
    const CommandBuffer& commandBuffer,
    const PipelineLayout& pipelineLayout) const {
  commandBuffer.CmdBindDescriptorSet(
      VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, sceneDescriptorSet,
      viewTransformBuffer.CalculateDynamicOffset(*bufferObjectIndex));
}

glm::mat4 Project(const float aspect) {
  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), aspect, 0.1f, 4000.0f);
  projection[1][1] *= -1;

  return projection;
}

void SceneDescriptor::UpdateAspect(const float aspect) {
  FrameData().projection = Project(aspect);
}
