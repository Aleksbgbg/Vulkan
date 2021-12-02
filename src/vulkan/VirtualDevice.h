#ifndef VULKAN_SRC_VULKAN_VIRTUALDEVICE_H_
#define VULKAN_SRC_VULKAN_VIRTUALDEVICE_H_

#include <vulkan/vulkan.h>

#include <string_view>
#include <vector>

#include "Buffer.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "DeviceMemory.h"
#include "Image.h"
#include "Pipeline.h"
#include "PipelineCache.h"
#include "PipelineLayout.h"
#include "Queue.h"
#include "RenderPass.h"
#include "Sampler.h"
#include "ShaderModule.h"
#include "Swapchain.h"
#include "structures/BufferCreateInfo.h"
#include "structures/DescriptorSetLayoutCreateInfo.h"
#include "structures/DeviceCreateInfo.h"
#include "structures/ImageCreateInfo.h"
#include "structures/RenderPassCreateInfo.h"
#include "structures/SwapchainCreateInfo.h"
#include "structures/WriteDescriptorSet.h"

class Surface;

class VirtualDevice {
  friend class ImGuiInstance;

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

  ShaderModule LoadShader(const VkShaderStageFlagBits stage,
                          const std::string_view shaderFilename) const;

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
  Pipeline CreateGraphicsPipeline(
      const PipelineCache& pipelineCache,
      const std::vector<ShaderModule>& shaders, PipelineLayout pipelineLayout,
      const SubpassReference subpassReference,
      const GraphicsPipelineCreateInfoBuilder& infoBuilder) const;
  DescriptorPool CreateDescriptorPool(
      const DescriptorPoolCreateInfoBuilder& infoBuilder) const;
  Sampler CreateSampler(SamplerCreateInfoBuilder& infoBuilder) const;
  PipelineCache CreatePipelineCache() const;
  PipelineCache LoadPipelineCache(const std::vector<u8>& data) const;

  void UpdateDescriptorSets(const u32 count,
                            const VkWriteDescriptorSet* const writes) const;

 private:
  VkDevice device;
};

#endif  // VULKAN_SRC_VULKAN_VIRTUALDEVICE_H_
