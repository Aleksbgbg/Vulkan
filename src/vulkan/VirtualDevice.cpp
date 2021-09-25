#include "VirtualDevice.h"

#include <fstream>

#include "util.h"
#include "error.h"
#include "Surface.h"

VirtualDevice::VirtualDevice(
    VkPhysicalDevice physicalDevice,
    VkPhysicalDeviceMemoryProperties* memoryProperties,
    DeviceCreateInfoBuilder& infoBuilder) : memoryProperties(memoryProperties) {
  PROCEED_ON_VALID_RESULT(vkCreateDevice(physicalDevice, infoBuilder.Build(), nullptr, &device))
}

VirtualDevice::~VirtualDevice() {
  if (device != nullptr) {
    vkDestroyDevice(device, nullptr);
  }
}

Queue VirtualDevice::GetQueue(const u32 familyIndex, const u32 queueIndex) const {
  VkQueue queue;
  vkGetDeviceQueue(device, familyIndex, queueIndex, &queue);
  return Queue(device, queue, familyIndex);
}

Buffer VirtualDevice::CreateBuffer(BufferCreateInfoBuilder& infoBuilder) const {
  return Buffer(device, memoryProperties, infoBuilder);
}

Image VirtualDevice::CreateImage(ImageCreateInfoBuilder& infoBuilder) const {
  return Image(device, memoryProperties, infoBuilder);
}

Swapchain VirtualDevice::CreateSwapchain(Surface& surface, SwapchainCreateInfoBuilder& infoBuilder) const {
  return Swapchain(device, infoBuilder.SetSurface(surface.surface));
}

ShaderModule VirtualDevice::LoadShader(const VkShaderStageFlagBits stage, const char* const shaderFilename) const {
  const std::vector<u8> code = ReadFile(shaderFilename);
  return ShaderModule(
      device,
      stage,
      ShaderModuleCreateInfoBuilder()
          .SetCodeSize(code.size())
          .SetPCode(reinterpret_cast<const u32*>(code.data())));
}

DescriptorSetLayout VirtualDevice::CreateDescriptorSetLayout(DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const {
  return DescriptorSetLayout(device, infoBuilder);
}

PipelineLayout VirtualDevice::CreatePipelineLayout(const DescriptorSetLayout& descriptorSetLayout) const {
  const std::vector<const DescriptorSetLayout*> layouts { &descriptorSetLayout };
  return PipelineLayout(device, layouts);
}

RenderPass VirtualDevice::CreateRenderPass(RenderPassCreateInfoBuilder& infoBuilder) const {
  return RenderPass(device, infoBuilder);
}

Pipeline VirtualDevice::CreateGraphicsPipeline(const std::vector<ShaderModule>& shaders,
                                               PipelineLayout pipelineLayout,
                                               RenderPass renderPass,
                                               GraphicsPipelineCreateInfoBuilder& infoBuilder) const {
  return Pipeline(device, shaders, std::move(pipelineLayout), std::move(renderPass), infoBuilder);
}

Semaphore VirtualDevice::MakeSemaphore() const {
  auto builder = SemaphoreCreateInfoBuilder();
  return Semaphore(device, builder);
}

void VirtualDevice::UpdateDescriptorSets(const u32 count, VkWriteDescriptorSet* writes) {
  vkUpdateDescriptorSets(device, count, writes, 0, nullptr);
}

DescriptorPool VirtualDevice::CreateDescriptorPool(DescriptorPoolCreateInfoBuilder& infoBuilder) const {
  return DescriptorPool(device, infoBuilder);
}

Fence VirtualDevice::MakeFence() const {
  return Fence(device);
}

Fence VirtualDevice::MakeFence(const VkFenceCreateFlags flags) const {
  return Fence(device, flags);
}

void VirtualDevice::WaitIdle() const {
  vkDeviceWaitIdle(device);
}

Sampler VirtualDevice::CreateSampler(SamplerCreateInfoBuilder& infoBuilder) const {
  return Sampler(device, infoBuilder);
}
