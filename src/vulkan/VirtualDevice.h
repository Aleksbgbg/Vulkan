#ifndef VULKAN_SRC_VULKAN_VIRTUALDEVICE_H
#define VULKAN_SRC_VULKAN_VIRTUALDEVICE_H

#include <vulkan/vulkan.h>
#include <vulkan/structures/BufferCreateInfo.h>
#include <vulkan/structures/SwapchainCreateInfo.h>
#include <vector>
#include <vulkan/structures/RenderPassCreateInfo.h>
#include <vulkan/structures/DescriptorSetLayoutCreateInfo.h>
#include <vulkan/structures/ImageCreateInfo.h>
#include <vulkan/structures/WriteDescriptorSet.h>

#include "lifetime_semantics.h"
#include "structures/DeviceCreateInfo.h"
#include "Buffer.h"
#include "Queue.h"
#include "Swapchain.h"
#include "Pipeline.h"
#include "ShaderModule.h"
#include "PipelineLayout.h"
#include "RenderPass.h"
#include "DescriptorSetLayout.h"
#include "Image.h"
#include "DescriptorPool.h"
#include "Sampler.h"
#include "memory/DeviceMemoryAllocator.h"

#undef CreateSemaphore

class Surface;

class VirtualDevice {
  friend class ImGuiInstance;
public:
  VirtualDevice() = default;
  explicit VirtualDevice(VkPhysicalDevice physicalDevice,
                         VkPhysicalDeviceMemoryProperties* memoryProperties,
                         DeviceCreateInfoBuilder& infoBuilder);

  VirtualDevice(const VirtualDevice&) = delete;
  VirtualDevice(VirtualDevice&& other)  noexcept : device(other.device), memoryProperties(other.memoryProperties) {
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

  ShaderModule LoadShader(const VkShaderStageFlagBits stage, const char* const shaderFilename) const;

  Semaphore CreateSemaphore() const;
  Fence CreateFence() const;
  Fence CreateFence(const VkFenceCreateFlags flags) const;

  Buffer CreateBuffer(BufferCreateInfoBuilder& infoBuilder) const;
  Swapchain CreateSwapchain(Surface& surface, SwapchainCreateInfoBuilder& infoBuilder) const;
  Image CreateImage(ImageCreateInfoBuilder& infoBuilder) const;
  DescriptorSetLayout CreateDescriptorSetLayout(DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const;
  PipelineLayout CreatePipelineLayout(const DescriptorSetLayout& descriptorSetLayout) const;
  PipelineLayout CreatePipelineLayout(
      const DescriptorSetLayout& descriptorSetLayout, PipelineLayoutCreateInfoBuilder& infoBuilder) const;
  PipelineLayout CreatePipelineLayout(const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts) const;
  RenderPass CreateRenderPass(RenderPassCreateInfoBuilder& infoBuilder) const;
  Pipeline CreateGraphicsPipeline(const std::vector<ShaderModule>& shaders,
                                  PipelineLayout pipelineLayout,
                                  const SubpassReference subpassReference,
                                  GraphicsPipelineCreateInfoBuilder& infoBuilder) const;
  DescriptorPool CreateDescriptorPool(DescriptorPoolCreateInfoBuilder& infoBuilder) const;
  Sampler CreateSampler(SamplerCreateInfoBuilder& infoBuilder) const;

  DeviceMemoryAllocator CreateMemoryAllocator() const;

  void UpdateDescriptorSets(const u32 count, VkWriteDescriptorSet* writes);

private:
  VkDevice device = nullptr;
  VkPhysicalDeviceMemoryProperties* memoryProperties;
};


#endif // VULKAN_SRC_VULKAN_VIRTUALDEVICE_H
