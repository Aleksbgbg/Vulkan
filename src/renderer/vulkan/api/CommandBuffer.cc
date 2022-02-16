#include "CommandBuffer.h"

#include "error.h"
#include "renderer/vulkan/api/structures/CommandBufferBeginInfo.h"
#include "renderer/vulkan/api/structures/CommandBufferInheritanceInfo.h"
#include "renderer/vulkan/api/structures/SubmitInfo.h"

namespace vk {

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
  this->~CommandBuffer();
  new (this) CommandBuffer(std::move(other));
  return *this;
}

void CommandBuffer::BeginWithInheritance(
    CommandBufferBeginInfoBuilder infoBuilder, const SubpassReference reference,
    const Framebuffer& framebuffer) const {
  Begin(infoBuilder.SetPInheritanceInfo(
      CommandBufferInheritanceInfoBuilder()
          .SetRenderPass(reference.renderPass->renderPass)
          .SetSubpass(reference.subpass)
          .SetFramebuffer(framebuffer.framebuffer)));
}

void CommandBuffer::Begin(
    const CommandBufferBeginInfoBuilder& infoBuilder) const {
  PROCEED_ON_VALID_RESULT(
      vkBeginCommandBuffer(commandBuffer, infoBuilder.Build()));
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

void CommandBuffer::SubmitCompute(const Fence& fence) const {
  PROCEED_ON_VALID_RESULT(vkQueueSubmit(queue, 1,
                                        SubmitInfoBuilder()
                                            .SetCommandBufferCount(1)
                                            .SetPCommandBuffers(&commandBuffer)
                                            .Build(),
                                        fence.Raw()));
}

void CommandBuffer::Submit(
    const SynchronisationPack& synchronisationPack) const {
  constexpr VkPipelineStageFlags flags[] = {
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

void CommandBuffer::CmdBindComputePipeline(
    const ComputePipeline& pipeline) const {
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                    pipeline.pipeline_);
}

void CommandBuffer::CmdDispatch(const u32 groupCountX, const u32 groupCountY,
                                const u32 groupCountZ) const {
  vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void CommandBuffer::CmdFillBuffer(const Buffer& buffer, const VkDeviceSize size,
                                  const u32 value) const {
  vkCmdFillBuffer(commandBuffer, buffer.buffer_, 0, size, value);
}

void CommandBuffer::CmdCopyBufferFull(const Buffer& source,
                                      const Buffer& dest) const {
  CmdCopyBuffer(source, dest, {.size = source.size_});
}

void CommandBuffer::CmdCopyBuffer(const Buffer& source, const Buffer& dest,
                                  const VkBufferCopy& copyRegion) const {
  vkCmdCopyBuffer(commandBuffer, source.buffer_, dest.buffer_, 1, &copyRegion);
}

void CommandBuffer::CmdCopyBufferToImage(
    const Buffer& source, const Image& dest, const VkImageLayout destLayout,
    const BufferImageCopyBuilder& infoBuilder) const {
  vkCmdCopyBufferToImage(commandBuffer, source.buffer_, dest.image, destLayout,
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
                       1, infoBuilder.SetBuffer(buffer.buffer_).Build(), 0,
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

void CommandBuffer::CmdSetViewport(const VkViewport& viewport) const {
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
}

void CommandBuffer::CmdSetScissor(const VkRect2D& scissor) const {
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
                                    const GraphicsPipeline& pipeline) const {
  vkCmdBindPipeline(commandBuffer, bindPoint, pipeline.pipeline);
}

void CommandBuffer::CmdPushConstants(const PipelineLayout& pipelineLayout,
                                     const VkShaderStageFlags shaderStageFlags,
                                     const u32 offset, const u32 size,
                                     const void* const values) const {
  vkCmdPushConstants(commandBuffer, pipelineLayout.pipelineLayout,
                     shaderStageFlags, offset, size, values);
}

void CommandBuffer::CmdBindVertexBuffers(const Buffer& buffer) const {
  const VkDeviceSize offset = 0;
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, &buffer.buffer_, &offset);
}

void CommandBuffer::CmdBindIndexBuffer(const Buffer& buffer,
                                       const VkDeviceSize offset,
                                       const VkIndexType indexType) const {
  vkCmdBindIndexBuffer(commandBuffer, buffer.buffer_, offset, indexType);
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

void CommandBuffer::CmdDraw(const u32 vertexCount,
                            const u32 instanceCount) const {
  vkCmdDraw(commandBuffer, vertexCount, instanceCount, 0, 0);
}

void CommandBuffer::CmdDrawIndexed(const u32 indexCount) const {
  CmdDrawIndexedInstanced(indexCount, 1);
}

void CommandBuffer::CmdDrawIndexedInstanced(const u32 indexCount,
                                            const u32 instanceCount) const {
  vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, 0, 0, 0);
}

void CommandBuffer::CmdDrawIndexedIndirect(const Buffer& buffer) const {
  vkCmdDrawIndexedIndirect(commandBuffer, buffer.buffer_, 0, 1, 0);
}

void CommandBuffer::CmdEndRenderPass() const {
  vkCmdEndRenderPass(commandBuffer);
}

void CommandBuffer::CmdNextSubpass(const VkSubpassContents contents) const {
  vkCmdNextSubpass(commandBuffer, contents);
}

void CommandBuffer::CmdExecuteCommands(const CommandBuffer& other) const {
  vkCmdExecuteCommands(commandBuffer, 1, &other.commandBuffer);
}

}  // namespace vk
