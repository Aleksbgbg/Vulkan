#ifndef VULKAN_SRC_VULKAN_COMMANDBUFFER_H
#define VULKAN_SRC_VULKAN_COMMANDBUFFER_H

#include <vulkan/vulkan.h>
#include <vulkan/structures/RenderPassBeginInfo.h>
#include <vulkan/structures/BufferImageCopy.h>
#include <vulkan/structures/ImageMemoryBarrier.h>

#include "lifetime_semantics.h"
#include "structures/CommandBufferAllocateInfo.h"
#include "Fence.h"
#include "Semaphore.h"
#include "Pipeline.h"
#include "Buffer.h"
#include "DescriptorSet.h"
#include "Framebuffer.h"
#include "SynchronisationPack.h"
#include "Image.h"

class CommandBuffer {
  friend class ImGuiInstance;
public:
  CommandBuffer() = default;
  explicit CommandBuffer(VkDevice device, VkQueue queue, VkCommandPool commandPool, CommandBufferAllocateInfoBuilder& infoBuilder);

  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer(CommandBuffer&& other) noexcept
    : device(other.device), queue(other.queue), commandPool(other.commandPool), commandBuffer(other.commandBuffer) {
    other.commandBuffer = nullptr;
  }

  ~CommandBuffer();

  CommandBuffer& operator=(const CommandBuffer&) = delete;
  CommandBuffer& operator=(CommandBuffer&& other) noexcept {
    device = other.device;
    queue = other.queue;
    commandPool = other.commandPool;
    std::swap(commandBuffer, other.commandBuffer);
    return *this;
  }

  void Begin();
  void BeginOneTimeSubmit();
  CommandBuffer& End();
  Fence Submit();
  void Submit(const SynchronisationPack& synchronisationPack);
  void Reset() const;
  void Reset(const VkCommandBufferResetFlags flags) const;

  void CmdCopyBufferFull(Buffer& source, Buffer& dest);
  void CmdCopyBuffer(Buffer& source, Buffer& dest, const VkBufferCopy& copyRegion);
  void CmdCopyBufferToImage(Buffer& source, Image& dest, const VkImageLayout destLayout, BufferImageCopyBuilder& infoBuilder);

  void CmdImageMemoryBarrier(
      Image& image,
      const VkPipelineStageFlags srcStageMask,
      const VkPipelineStageFlags dstStageMask,
      ImageMemoryBarrierBuilder& infoBuilder);

  void CmdBeginRenderPass(RenderPassBeginInfoBuilder& infoBuilder, const VkSubpassContents subpassContents, RenderPass& renderPass, Framebuffer& framebuffer);
  void CmdBindPipeline(const VkPipelineBindPoint bindPoint, Pipeline& pipeline);
  void CmdPushConstants(
      PipelineLayout& pipelineLayout,
      const VkShaderStageFlags shaderStageFlags,
      const u32 offset,
      const u32 size,
      void* values);
  void CmdBindVertexBuffers(Buffer& buffer, const u32 binding);
  void CmdBindIndexBuffer(Buffer& buffer, const VkIndexType indexType);
  void CmdBindDescriptorSets(const VkPipelineBindPoint bindPoint, PipelineLayout& pipelineLayout, DescriptorSet& descriptorSet);
  void CmdDrawIndexed(const u32 indexCount, const u32 instanceCount);
  void CmdNextSubpass(const VkSubpassContents contents);
  void CmdEndRenderPass();

private:
  VkDevice device;
  VkQueue queue;
  VkCommandPool commandPool;
  VkCommandBuffer commandBuffer = nullptr;
};

#endif // VULKAN_SRC_VULKAN_COMMANDBUFFER_H
