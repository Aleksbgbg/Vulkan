#ifndef SRC_RENDERER_VULKAN_API_VIRTUALDEVICE_H_
#define SRC_RENDERER_VULKAN_API_VIRTUALDEVICE_H_

#include <vulkan/vulkan.h>

#include <string_view>
#include <vector>

#include "Buffer.h"
#include "ComputePipeline.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "DeviceMemory.h"
#include "GraphicsPipeline.h"
#include "Image.h"
#include "PipelineCache.h"
#include "PipelineLayout.h"
#include "Queue.h"
#include "RenderPass.h"
#include "Sampler.h"
#include "ShaderModule.h"
#include "Swapchain.h"
#include "renderer/vulkan/api/structures/BufferCreateInfo.h"
#include "renderer/vulkan/api/structures/ComputePipelineCreateInfo.h"
#include "renderer/vulkan/api/structures/DescriptorSetLayoutCreateInfo.h"
#include "renderer/vulkan/api/structures/DeviceCreateInfo.h"
#include "renderer/vulkan/api/structures/ImageCreateInfo.h"
#include "renderer/vulkan/api/structures/RenderPassCreateInfo.h"
#include "renderer/vulkan/api/structures/SwapchainCreateInfo.h"
#include "renderer/vulkan/api/structures/WriteDescriptorSet.h"

namespace vk {

class Surface;

class VirtualDevice {
 public:
  VirtualDevice();
  VirtualDevice(VkPhysicalDevice physicalDevice,
                const DeviceCreateInfoBuilder& infoBuilder);

  VirtualDevice(const VirtualDevice&) = delete;
  VirtualDevice(VirtualDevice&& other) noexcept;

  ~VirtualDevice();

  VirtualDevice& operator=(const VirtualDevice&) = delete;
  VirtualDevice& operator=(VirtualDevice&& other) noexcept;

  void WaitIdle() const;

  Queue GetQueue(const u32 familyIndex, const u32 queueIndex) const;

  ShaderModule LoadComputeShader(const std::vector<u8>& code) const;
  ShaderModule LoadShader(const VkShaderStageFlagBits stage,
                          const std::vector<u8>& code) const;

  Semaphore CreateSemaphore() const;
  Fence CreateFence() const;
  Fence CreateFence(const VkFenceCreateFlags flags) const;

  DeviceMemory AllocateDeviceMemory(const u32 memoryTypeIndex,
                                    const VkDeviceSize size) const;

  Buffer CreateBuffer(BufferCreateInfoBuilder& infoBuilder) const;
  Swapchain CreateSwapchain(const Surface& surface,
                            SwapchainCreateInfoBuilder infoBuilder) const;
  Swapchain CreateSwapchain(const Surface& surface,
                            const Swapchain& oldSwapchain,
                            SwapchainCreateInfoBuilder infoBuilder) const;
  Image CreateImage(ImageCreateInfoBuilder& infoBuilder) const;
  DescriptorSetLayout CreateDescriptorSetLayout(
      const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const;
  PipelineLayout CreatePipelineLayout(
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
      const PipelineLayoutCreateInfoBuilder& infoBuilder) const;
  RenderPass CreateRenderPass(RenderPassCreateInfoBuilder& infoBuilder) const;
  GraphicsPipeline CreateGraphicsPipeline(
      const PipelineCache& pipelineCache,
      const std::vector<ShaderModule>& shaders, PipelineLayout pipelineLayout,
      const SubpassReference subpassReference,
      const GraphicsPipelineCreateInfoBuilder& infoBuilder) const;
  DescriptorPool CreateDescriptorPool(
      const DescriptorPoolCreateInfoBuilder& infoBuilder) const;
  Sampler CreateSampler(SamplerCreateInfoBuilder& infoBuilder) const;
  PipelineCache CreatePipelineCache() const;
  PipelineCache LoadPipelineCache(const std::vector<u8>& data) const;

  ComputePipeline CreateComputePipeline(const PipelineCache& pipelineCache,
                                        PipelineLayout pipelineLayout,
                                        const ShaderModule& shaderModule) const;

  void UpdateDescriptorSets(const u32 count,
                            const VkWriteDescriptorSet* const writes) const;
  void UpdateDescriptorSets(
      const DescriptorSet::WriteDescriptorSet& descriptorSetWrite) const;
  void UpdateDescriptorSets(
      const std::vector<DescriptorSet::WriteDescriptorSet>& descriptorSetWrites)
      const;

 private:
  VkDevice device;
};

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_VIRTUALDEVICE_H_
