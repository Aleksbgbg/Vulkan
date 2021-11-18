#include "CommandBuffer.h"

#include <vulkan/structures/SubmitInfo.h>

#include "error.h"
#include "structures/CommandBufferBeginInfo.h"

CommandBuffer::CommandBuffer() : commandBuffer(nullptr) {}

CommandBuffer::CommandBuffer(VkDevice device, VkQueue queue,
                             VkCommandPool commandPool,
                             CommandBufferAllocateInfoBuilder& infoBuilder)
    : device(device), queue(queue), commandPool(commandPool) {
  PROCEED_ON_VALID_RESULT(vkAllocateCommandBuffers(
      device, infoBuilder.SetCommandBufferCount(1).Build(), &commandBuffer));
}

CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept
    : device(other.device),
      queue(other.queue),
      commandPool(other.commandPool),
      commandBuffer(other.commandBuffer) {
  other.commandBuffer = nullptr;
}

CommandBuffer::~CommandBuffer() {
  if (commandBuffer != nullptr) {
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
  }
}

CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept {
  std::swap(device, other.device);
  queue = other.queue;
  std::swap(commandPool, other.commandPool);
  std::swap(commandBuffer, other.commandBuffer);
  return *this;
}

void CommandBuffer::Begin() const {
  PROCEED_ON_VALID_RESULT(vkBeginCommandBuffer(
      commandBuffer, CommandBufferBeginInfoBuilder().Build()));
}

void CommandBuffer::BeginOneTimeSubmit() const {
  PROCEED_ON_VALID_RESULT(vkBeginCommandBuffer(
      commandBuffer, CommandBufferBeginInfoBuilder()
                         .SetFlags(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)
                         .Build()));
}

const CommandBuffer& CommandBuffer::End() const {
  PROCEED_ON_VALID_RESULT(vkEndCommandBuffer(commandBuffer));
  return *this;
}

const Fence& CommandBuffer::Submit(const Fence& fence) const {
  PROCEED_ON_VALID_RESULT(vkQueueSubmit(queue, 1,
                                        SubmitInfoBuilder()
                                            .SetCommandBufferCount(1)
                                            .SetPCommandBuffers(&commandBuffer)
                                            .Build(),
                                        fence.Raw()));
  return fence;
}

void CommandBuffer::Submit(
    const SynchronisationPack& synchronisationPack) const {
  VkPipelineStageFlags flags[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  PROCEED_ON_VALID_RESULT(vkQueueSubmit(
      queue, 1,
      SubmitInfoBuilder()
          .SetCommandBufferCount(1)
          .SetPCommandBuffers(&commandBuffer)
          .SetWaitSemaphoreCount(1)
          .SetPWaitSemaphores(synchronisationPack.GetWaitSemaphore()->Pointer())
          .SetPWaitDstStageMask(flags)
          .SetSignalSemaphoreCount(1)
          .SetPSignalSemaphores(
              synchronisationPack.GetSignalSemaphore()->Pointer())
          .Build(),
      synchronisationPack.GetSignalFence()->Raw()));
}

void CommandBuffer::Reset() const {
  Reset(0);
}

void CommandBuffer::Reset(const VkCommandBufferResetFlags flags) const {
  vkResetCommandBuffer(commandBuffer, flags);
}

void CommandBuffer::CmdCopyBufferFull(Buffer& source, Buffer& dest) const {
  CmdCopyBuffer(source, dest, {.size = source.size});
}

void CommandBuffer::CmdCopyBuffer(Buffer& source, Buffer& dest,
                                  const VkBufferCopy& copyRegion) const {
  vkCmdCopyBuffer(commandBuffer, source.buffer, dest.buffer, 1, &copyRegion);
}

void CommandBuffer::CmdCopyBufferToImage(
    const Buffer& source, const Image& dest, const VkImageLayout destLayout,
    const BufferImageCopyBuilder& infoBuilder) const {
  vkCmdCopyBufferToImage(commandBuffer, source.buffer, dest.image, destLayout,
                         1, infoBuilder.Build());
}

void CommandBuffer::CmdGlobalMemoryBarrier(
    const VkPipelineStageFlags srcStageMask,
    const VkPipelineStageFlags dstStageMask,
    MemoryBarrierBuilder& infoBuilder) const {
  vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0, 1,
                       infoBuilder.Build(), 0, nullptr, 0, nullptr);
}

void CommandBuffer::CmdBufferMemoryBarrier(
    const Buffer& buffer, const VkPipelineStageFlags srcStageMask,
    const VkPipelineStageFlags dstStageMask,
    BufferMemoryBarrierBuilder& infoBuilder) const {
  vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr,
                       1, infoBuilder.SetBuffer(buffer.buffer).Build(), 0,
                       nullptr);
}

void CommandBuffer::CmdImageMemoryBarrier(
    const Image& image, const VkPipelineStageFlags srcStageMask,
    const VkPipelineStageFlags dstStageMask,
    ImageMemoryBarrierBuilder& infoBuilder) const {
  vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr,
                       0, nullptr, 1,
                       infoBuilder.SetImage(image.image).Build());
}

void CommandBuffer::CmdSetViewport(const VkViewport viewport) const {
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
}

void CommandBuffer::CmdSetScissor(const VkRect2D scissor) const {
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void CommandBuffer::CmdBeginRenderPass(RenderPassBeginInfoBuilder infoBuilder,
                                       const VkSubpassContents subpassContents,
                                       const RenderPass& renderPass,
                                       const Framebuffer& framebuffer) const {
  vkCmdBeginRenderPass(commandBuffer,
                       infoBuilder.SetRenderPass(renderPass.renderPass)
                           .SetFramebuffer(framebuffer.framebuffer)
                           .Build(),
                       subpassContents);
}

void CommandBuffer::CmdBindPipeline(const VkPipelineBindPoint bindPoint,
                                    const Pipeline& pipeline) const {
  vkCmdBindPipeline(commandBuffer, bindPoint, pipeline.pipeline);
}

void CommandBuffer::CmdPushConstants(const PipelineLayout& pipelineLayout,
                                     const VkShaderStageFlags shaderStageFlags,
                                     const u32 offset, const u32 size,
                                     const void* const values) const {
  vkCmdPushConstants(commandBuffer, pipelineLayout.pipelineLayout,
                     shaderStageFlags, offset, size, values);
}

void CommandBuffer::CmdBindVertexBuffers(const Buffer& buffer,
                                         const u32 binding) const {
  VkDeviceSize offset = 0;
  vkCmdBindVertexBuffers(commandBuffer, binding, 1, &buffer.buffer, &offset);
}

void CommandBuffer::CmdBindIndexBuffer(const Buffer& buffer,
                                       const VkIndexType indexType) const {
  vkCmdBindIndexBuffer(commandBuffer, buffer.buffer, 0, indexType);
}

void CommandBuffer::CmdBindDescriptorSet(
    const VkPipelineBindPoint bindPoint, const PipelineLayout& pipelineLayout,
    const u32 setIndex, const DescriptorSet& descriptorSet) const {
  vkCmdBindDescriptorSets(commandBuffer, bindPoint,
                          pipelineLayout.pipelineLayout, setIndex, 1,
                          &descriptorSet.descriptorSet, 0, nullptr);
}

void CommandBuffer::CmdBindDescriptorSet(const VkPipelineBindPoint bindPoint,
                                         const PipelineLayout& pipelineLayout,
                                         const u32 setIndex,
                                         const DescriptorSet& descriptorSet,
                                         const u32 dynamicOffset) const {
  vkCmdBindDescriptorSets(commandBuffer, bindPoint,
                          pipelineLayout.pipelineLayout, setIndex, 1,
                          &descriptorSet.descriptorSet, 1, &dynamicOffset);
}

void CommandBuffer::CmdDrawIndexed(const u32 indexCount) const {
  CmdDrawIndexedInstanced(indexCount, 1);
}

void CommandBuffer::CmdDrawIndexedInstanced(const u32 indexCount,
                                            const u32 instanceCount) const {
  vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, 0, 0, 0);
}

void CommandBuffer::CmdEndRenderPass() const {
  vkCmdEndRenderPass(commandBuffer);
}

void CommandBuffer::CmdNextSubpass(const VkSubpassContents contents) const {
  vkCmdNextSubpass(commandBuffer, contents);
}
