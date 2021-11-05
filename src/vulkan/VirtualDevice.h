#ifndef VULKAN_SRC_VULKAN_VIRTUALDEVICE_H
#define VULKAN_SRC_VULKAN_VIRTUALDEVICE_H

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
#include "lifetime_semantics.h"
#include "structures/BufferCreateInfo.h"
#include "structures/DescriptorSetLayoutCreateInfo.h"
#include "structures/DeviceCreateInfo.h"
#include "structures/ImageCreateInfo.h"
#include "structures/RenderPassCreateInfo.h"
#include "structures/SwapchainCreateInfo.h"
#include "structures/WriteDescriptorSet.h"
#include "util/include/windows.h"

class Surface;

class VirtualDevice {
  friend class ImGuiInstance;

 public:
  VirtualDevice() = default;
  explicit VirtualDevice(VkPhysicalDevice physicalDevice,
                         VkPhysicalDeviceMemoryProperties* memoryProperties,
                         DeviceCreateInfoBuilder& infoBuilder);

  VirtualDevice(const VirtualDevice&) = delete;
  VirtualDevice(VirtualDevice&& other) noexcept
      : device(other.device), memoryProperties(other.memoryProperties) {
    device = nullptr;
  }

  ~VirtualDevice();

  VirtualDevice& operator=(const VirtualDevice&) = delete;
  VirtualDevice& operator=(VirtualDevice&& other) noexcept {
    std::swap(device, other.device);
    memoryProperties = other.memoryProperties;
    return *this;
  }

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
  Swapchain CreateSwapchain(Surface& surface,
                            SwapchainCreateInfoBuilder& infoBuilder) const;
  Swapchain CreateSwapchain(Surface& surface, const Swapchain& oldSwapchain,
                            SwapchainCreateInfoBuilder& infoBuilder) const;
  Image CreateImage(ImageCreateInfoBuilder& infoBuilder) const;
  DescriptorSetLayout CreateDescriptorSetLayout(
      DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const;
  PipelineLayout CreatePipelineLayout(
      const DescriptorSetLayout& descriptorSetLayout) const;
  PipelineLayout CreatePipelineLayout(
      const DescriptorSetLayout& descriptorSetLayout,
      PipelineLayoutCreateInfoBuilder& infoBuilder) const;
  PipelineLayout CreatePipelineLayout(
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts)
      const;
  PipelineLayout CreatePipelineLayout(
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
      PipelineLayoutCreateInfoBuilder& infoBuilder) const;
  RenderPass CreateRenderPass(RenderPassCreateInfoBuilder& infoBuilder) const;
  Pipeline CreateGraphicsPipeline(
      const PipelineCache& pipelineCache,
      const std::vector<ShaderModule>& shaders, PipelineLayout pipelineLayout,
      const SubpassReference subpassReference,
      GraphicsPipelineCreateInfoBuilder& infoBuilder) const;
  DescriptorPool CreateDescriptorPool(
      DescriptorPoolCreateInfoBuilder& infoBuilder) const;
  Sampler CreateSampler(SamplerCreateInfoBuilder& infoBuilder) const;
  PipelineCache CreatePipelineCache() const;
  PipelineCache LoadPipelineCache(const std::vector<u8>& data) const;

  void UpdateDescriptorSets(const u32 count,
                            VkWriteDescriptorSet* writes) const;

 private:
  VkDevice device = nullptr;
  VkPhysicalDeviceMemoryProperties* memoryProperties;
};

#endif  // VULKAN_SRC_VULKAN_VIRTUALDEVICE_H
