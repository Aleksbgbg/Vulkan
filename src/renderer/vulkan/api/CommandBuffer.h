#ifndef VULKAN_SRC_RENDERER_VULKAN_API_COMMANDBUFFER_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_COMMANDBUFFER_H_

#include <vulkan/vulkan.h>

#include "Buffer.h"
#include "ComputePipeline.h"
#include "DescriptorSet.h"
#include "Fence.h"
#include "Framebuffer.h"
#include "Image.h"
#include "Pipeline.h"
#include "Semaphore.h"
#include "SynchronisationPack.h"
#include "renderer/vulkan/api/structures/BufferImageCopy.h"
#include "renderer/vulkan/api/structures/BufferMemoryBarrier.h"
#include "renderer/vulkan/api/structures/CommandBufferAllocateInfo.h"
#include "renderer/vulkan/api/structures/ImageMemoryBarrier.h"
#include "renderer/vulkan/api/structures/MemoryBarrier.h"
#include "renderer/vulkan/api/structures/RenderPassBeginInfo.h"

class CommandBuffer {
  friend class ImGuiInstance;

 public:
  CommandBuffer();
  CommandBuffer(VkDevice device, VkQueue queue, VkCommandPool commandPool,
                CommandBufferAllocateInfoBuilder& infoBuilder);

  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer(CommandBuffer&& other) noexcept;

  ~CommandBuffer();

  CommandBuffer& operator=(const CommandBuffer&) = delete;
  CommandBuffer& operator=(CommandBuffer&& other) noexcept;

  void Begin() const;
  void BeginOneTimeSubmit() const;
  const CommandBuffer& End() const;
  const Fence& Submit(const Fence& fence) const;
  void SubmitCompute(const Fence& fence) const;
  void Submit(const SynchronisationPack& synchronisationPack) const;
  void Reset() const;
  void Reset(const VkCommandBufferResetFlags flags) const;

  void CmdBindComputePipeline(const ComputePipeline& pipeline) const;
  void CmdDispatch(const u32 groupCountX, const u32 groupCountY,
                   const u32 groupCountZ) const;

  void CmdCopyBufferFull(Buffer& source, Buffer& dest) const;
  void CmdCopyBuffer(Buffer& source, Buffer& dest,
                     const VkBufferCopy& copyRegion) const;
  void CmdCopyBufferToImage(const Buffer& source, const Image& dest,
                            const VkImageLayout destLayout,
                            const BufferImageCopyBuilder& infoBuilder) const;

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

  void CmdSetViewport(const VkViewport& viewport) const;
  void CmdSetScissor(const VkRect2D& scissor) const;

  void CmdBeginRenderPass(RenderPassBeginInfoBuilder infoBuilder,
                          const VkSubpassContents subpassContents,
                          const RenderPass& renderPass,
                          const Framebuffer& framebuffer) const;
  void CmdBindPipeline(const VkPipelineBindPoint bindPoint,
                       const Pipeline& pipeline) const;
  void CmdPushConstants(const PipelineLayout& pipelineLayout,
                        const VkShaderStageFlags shaderStageFlags,
                        const u32 offset, const u32 size,
                        const void* const values) const;
  void CmdBindVertexBuffers(const Buffer& buffer, const u32 binding) const;
  void CmdBindIndexBuffer(const Buffer& buffer,
                          const VkIndexType indexType) const;

  void CmdBindDescriptorSet(const VkPipelineBindPoint bindPoint,
                            const PipelineLayout& pipelineLayout,
                            const u32 setIndex,
                            const DescriptorSet& descriptorSet) const;
  void CmdBindDescriptorSet(const VkPipelineBindPoint bindPoint,
                            const PipelineLayout& pipelineLayout,
                            const u32 setIndex,
                            const DescriptorSet& descriptorSet,
                            const u32 dynamicOffset) const;

  void CmdDrawIndexed(const u32 indexCount) const;
  void CmdDrawIndexedInstanced(const u32 indexCount,
                               const u32 instanceCount) const;
  void CmdDrawIndexedIndirect(const Buffer& buffer) const;

  void CmdNextSubpass(const VkSubpassContents contents) const;
  void CmdEndRenderPass() const;

 private:
  VkDevice device;
  VkQueue queue;
  VkCommandPool commandPool;
  VkCommandBuffer commandBuffer;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_COMMANDBUFFER_H_
