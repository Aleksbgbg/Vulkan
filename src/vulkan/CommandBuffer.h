#ifndef VULKAN_SRC_VULKAN_COMMANDBUFFER_H
#define VULKAN_SRC_VULKAN_COMMANDBUFFER_H

#include <vulkan/structures/BufferMemoryBarrier.h>
#include <vulkan/structures/MemoryBarrier.h>
#include <vulkan/vulkan.h>

#include "Buffer.h"
#include "DescriptorSet.h"
#include "Fence.h"
#include "Framebuffer.h"
#include "Image.h"
#include "Pipeline.h"
#include "Semaphore.h"
#include "SynchronisationPack.h"
#include "lifetime_semantics.h"
#include "structures/BufferImageCopy.h"
#include "structures/CommandBufferAllocateInfo.h"
#include "structures/ImageMemoryBarrier.h"
#include "structures/RenderPassBeginInfo.h"

class CommandBuffer {
  friend class ImGuiInstance;

 public:
  CommandBuffer() = default;
  explicit CommandBuffer(VkDevice device, VkQueue queue,
                         VkCommandPool commandPool,
                         CommandBufferAllocateInfoBuilder& infoBuilder);

  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer(CommandBuffer&& other) noexcept
      : device(other.device),
        queue(other.queue),
        commandPool(other.commandPool),
        commandBuffer(other.commandBuffer) {
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
  const Fence& Submit(const Fence& fence);
  void Submit(const SynchronisationPack& synchronisationPack);
  void Reset() const;
  void Reset(const VkCommandBufferResetFlags flags) const;

  void CmdCopyBufferFull(Buffer& source, Buffer& dest);
  void CmdCopyBuffer(Buffer& source, Buffer& dest,
                     const VkBufferCopy& copyRegion);
  void CmdCopyBufferToImage(Buffer& source, Image& dest,
                            const VkImageLayout destLayout,
                            BufferImageCopyBuilder& infoBuilder);

  void CmdGlobalMemoryBarrier(const VkPipelineStageFlags srcStageMask,
                              const VkPipelineStageFlags dstStageMask,
                              MemoryBarrierBuilder& infoBuilder) const;
  void CmdBufferMemoryBarrier(const Buffer& buffer,
                              const VkPipelineStageFlags srcStageMask,
                              const VkPipelineStageFlags dstStageMask,
                              BufferMemoryBarrierBuilder& infoBuilder) const;
  void CmdImageMemoryBarrier(const Image& image,
                             const VkPipelineStageFlags srcStageMask,
                             const VkPipelineStageFlags dstStageMask,
                             ImageMemoryBarrierBuilder& infoBuilder) const;

  void CmdBeginRenderPass(RenderPassBeginInfoBuilder& infoBuilder,
                          const VkSubpassContents subpassContents,
                          RenderPass& renderPass, Framebuffer& framebuffer);
  void CmdBindPipeline(const VkPipelineBindPoint bindPoint, Pipeline& pipeline);
  void CmdPushConstants(PipelineLayout& pipelineLayout,
                        const VkShaderStageFlags shaderStageFlags,
                        const u32 offset, const u32 size, void* values);
  void CmdBindVertexBuffers(Buffer& buffer, const u32 binding);
  void CmdBindIndexBuffer(Buffer& buffer, const VkIndexType indexType);
  void CmdBindDescriptorSets(const VkPipelineBindPoint bindPoint,
                             const PipelineLayout& pipelineLayout,
                             const u32 firstSet, const u32 descriptorSetCount,
                             const DescriptorSet& descriptorSet) const;
  void CmdDrawIndexed(const u32 indexCount, const u32 instanceCount);
  void CmdNextSubpass(const VkSubpassContents contents);
  void CmdEndRenderPass();

 private:
  VkDevice device;
  VkQueue queue;
  VkCommandPool commandPool;
  VkCommandBuffer commandBuffer = nullptr;
};

#endif  // VULKAN_SRC_VULKAN_COMMANDBUFFER_H
