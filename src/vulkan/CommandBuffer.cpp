#include <vulkan/structures/SubmitInfo.h>
#include "CommandBuffer.h"

#include "error.h"
#include "structures/CommandBufferBeginInfo.h"

CommandBuffer::CommandBuffer(VkDevice device, VkQueue queue, VkCommandPool commandPool, CommandBufferAllocateInfoBuilder& infoBuilder)
  : device(device), queue(queue), commandPool(commandPool) {
  PROCEED_ON_VALID_RESULT(
      vkAllocateCommandBuffers(device, infoBuilder.SetCommandBufferCount(1).Build(), &commandBuffer))
}

CommandBuffer::~CommandBuffer() {
  if (commandBuffer != nullptr) {
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
  }
}

void CommandBuffer::Begin() {
  PROCEED_ON_VALID_RESULT(vkBeginCommandBuffer(commandBuffer, CommandBufferBeginInfoBuilder().Build()))
}

void CommandBuffer::BeginOneTimeSubmit() {
  PROCEED_ON_VALID_RESULT(
      vkBeginCommandBuffer(
          commandBuffer,
          CommandBufferBeginInfoBuilder().SetFlags(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT).Build()))
}

CommandBuffer& CommandBuffer::End() {
  PROCEED_ON_VALID_RESULT(vkEndCommandBuffer(commandBuffer))
  return *this;
}

Fence CommandBuffer::Submit() {
  Fence fence(device);
  PROCEED_ON_VALID_RESULT(
      vkQueueSubmit(
          queue,
          1,
          SubmitInfoBuilder()
              .SetCommandBufferCount(1)
              .SetPCommandBuffers(&commandBuffer)
              .Build(),
          fence.Raw()))
  return fence;
}

void CommandBuffer::Submit(const SynchronisationPack& synchronisationPack) {
  VkPipelineStageFlags flags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  PROCEED_ON_VALID_RESULT(
      vkQueueSubmit(
          queue,
          1,
          SubmitInfoBuilder()
              .SetCommandBufferCount(1)
              .SetPCommandBuffers(&commandBuffer)
              .SetWaitSemaphoreCount(1)
              .SetPWaitSemaphores(synchronisationPack.GetWaitSemaphore()->Pointer())
              .SetPWaitDstStageMask(flags)
              .SetSignalSemaphoreCount(1)
              .SetPSignalSemaphores(synchronisationPack.GetSignalSemaphore()->Pointer())
              .Build(),
          synchronisationPack.GetSignalFence()->Raw()))
}

void CommandBuffer::Reset() const {
  Reset(0);
}

void CommandBuffer::Reset(const VkCommandBufferResetFlags flags) const {
  vkResetCommandBuffer(commandBuffer, flags);
}

void CommandBuffer::CmdCopyBufferFull(Buffer& source, Buffer& dest) {
  CmdCopyBuffer(source, dest, { .size = source.size });
}

void CommandBuffer::CmdCopyBuffer(Buffer& source, Buffer& dest, const VkBufferCopy& copyRegion) {
  vkCmdCopyBuffer(commandBuffer, source.buffer, dest.buffer, 1, &copyRegion);
}

void CommandBuffer::CmdCopyBufferToImage(Buffer& source, Image& dest, const VkImageLayout destLayout, BufferImageCopyBuilder& infoBuilder) {
  vkCmdCopyBufferToImage(commandBuffer, source.buffer, dest.image, destLayout, 1, infoBuilder.Build());
}

void CommandBuffer::CmdImageMemoryBarrier(
    Image& image,
    const VkPipelineStageFlags srcStageMask,
    const VkPipelineStageFlags dstStageMask,
    ImageMemoryBarrierBuilder& infoBuilder) {
  vkCmdPipelineBarrier(
      commandBuffer,
      srcStageMask,
      dstStageMask,
      0,
      0,
      nullptr,
      0,
      nullptr,
      1,
      infoBuilder.SetImage(image.image).Build());
}

void CommandBuffer::CmdBeginRenderPass(RenderPassBeginInfoBuilder& infoBuilder, const VkSubpassContents subpassContents,
                                       RenderPass& renderPass, Framebuffer& framebuffer) {
  vkCmdBeginRenderPass(
      commandBuffer,
      infoBuilder.SetRenderPass(renderPass.renderPass).SetFramebuffer(framebuffer.framebuffer).Build(),
      subpassContents);
}

void CommandBuffer::CmdBindPipeline(const VkPipelineBindPoint bindPoint, Pipeline& pipeline) {
  vkCmdBindPipeline(commandBuffer, bindPoint, pipeline.pipeline);
}

void CommandBuffer::CmdPushConstants(PipelineLayout& pipelineLayout, const VkShaderStageFlags shaderStageFlags,
                                     const u32 offset, const u32 size, void* values) {
  vkCmdPushConstants(commandBuffer, pipelineLayout.pipelineLayout, shaderStageFlags, offset, size, values);
}

void CommandBuffer::CmdBindVertexBuffers(Buffer& buffer, const u32 binding) {
  VkDeviceSize offset = 0;
  vkCmdBindVertexBuffers(commandBuffer, binding, 1, &buffer.buffer, &offset);
}

void CommandBuffer::CmdBindIndexBuffer(Buffer& buffer, const VkIndexType indexType) {
  vkCmdBindIndexBuffer(commandBuffer, buffer.buffer, 0, indexType);
}

void CommandBuffer::CmdBindDescriptorSets(const VkPipelineBindPoint bindPoint, PipelineLayout& pipelineLayout,
                                          DescriptorSet& descriptorSet) {
  vkCmdBindDescriptorSets(
      commandBuffer,
      bindPoint,
      pipelineLayout.pipelineLayout,
      /* firstSet= */ 0,
      /* descriptorSetCount= */ 1,
      &descriptorSet.descriptorSet,
      /* dynamicOffsetCount= */ 0,
      /* pDynamicOffsets= */ nullptr);
}

void CommandBuffer::CmdDrawIndexed(const u32 indexCount, const u32 instanceCount) {
  vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, 0, 0, 0);
}

void CommandBuffer::CmdEndRenderPass() {
  vkCmdEndRenderPass(commandBuffer);
}

void CommandBuffer::CmdNextSubpass(const VkSubpassContents contents) {
  vkCmdNextSubpass(commandBuffer, contents);
}
