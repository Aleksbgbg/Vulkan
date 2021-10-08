#include "VirtualDevice.h"

#include "Surface.h"
#include "error.h"
#include "util.h"

#undef CreateSemaphore

VirtualDevice::VirtualDevice(VkPhysicalDevice physicalDevice,
                             VkPhysicalDeviceMemoryProperties* memoryProperties,
                             DeviceCreateInfoBuilder& infoBuilder)
    : memoryProperties(memoryProperties) {
  PROCEED_ON_VALID_RESULT(
      vkCreateDevice(physicalDevice, infoBuilder.Build(), nullptr, &device));
}

VirtualDevice::~VirtualDevice() {
  if (device != nullptr) {
    vkDestroyDevice(device, nullptr);
  }
}

Queue VirtualDevice::GetQueue(const u32 familyIndex,
                              const u32 queueIndex) const {
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

Swapchain VirtualDevice::CreateSwapchain(
    Surface& surface, SwapchainCreateInfoBuilder& infoBuilder) const {
  return Swapchain(device, infoBuilder.SetSurface(surface.surface));
}

ShaderModule VirtualDevice::LoadShader(const VkShaderStageFlagBits stage,
                                       const char* const shaderFilename) const {
  const std::vector<u8> code = ReadFile(shaderFilename);
  return ShaderModule(device, stage,
                      ShaderModuleCreateInfoBuilder()
                          .SetCodeSize(code.size())
                          .SetPCode(reinterpret_cast<const u32*>(code.data())));
}

DeviceMemory VirtualDevice::AllocateDeviceMemory(
    const u32 memoryTypeIndex, const VkDeviceSize size) const {
  return DeviceMemory(
      device,
      MemoryAllocateInfoBuilder().SetAllocationSize(size).SetMemoryTypeIndex(
          memoryTypeIndex));
}

DescriptorSetLayout VirtualDevice::CreateDescriptorSetLayout(
    DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const {
  return DescriptorSetLayout(device, infoBuilder);
}

PipelineLayout VirtualDevice::CreatePipelineLayout(
    const DescriptorSetLayout& descriptorSetLayout) const {
  return CreatePipelineLayout({&descriptorSetLayout});
}

PipelineLayout VirtualDevice::CreatePipelineLayout(
    const DescriptorSetLayout& descriptorSetLayout,
    PipelineLayoutCreateInfoBuilder& infoBuilder) const {
  return PipelineLayout(device, infoBuilder, {&descriptorSetLayout});
}

PipelineLayout VirtualDevice::CreatePipelineLayout(
    const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts) const {
  return PipelineLayout(device, descriptorSetLayouts);
}

RenderPass VirtualDevice::CreateRenderPass(
    RenderPassCreateInfoBuilder& infoBuilder) const {
  return RenderPass(device, infoBuilder);
}

Pipeline VirtualDevice::CreateGraphicsPipeline(
    const std::vector<ShaderModule>& shaders, PipelineLayout pipelineLayout,
    const SubpassReference subpassReference,
    GraphicsPipelineCreateInfoBuilder& infoBuilder) const {
  return Pipeline(device, shaders, std::move(pipelineLayout), subpassReference,
                  infoBuilder);
}

Semaphore VirtualDevice::CreateSemaphore() const {
  return Semaphore(device, SemaphoreCreateInfoBuilder().Reference());
}

void VirtualDevice::UpdateDescriptorSets(const u32 count,
                                         VkWriteDescriptorSet* writes) {
  vkUpdateDescriptorSets(device, count, writes, 0, nullptr);
}

DescriptorPool VirtualDevice::CreateDescriptorPool(
    DescriptorPoolCreateInfoBuilder& infoBuilder) const {
  return DescriptorPool(device, infoBuilder);
}

Fence VirtualDevice::CreateFence() const {
  return Fence(device);
}

Fence VirtualDevice::CreateFence(const VkFenceCreateFlags flags) const {
  return Fence(device, flags);
}

void VirtualDevice::WaitIdle() const {
  vkDeviceWaitIdle(device);
}

Sampler VirtualDevice::CreateSampler(
    SamplerCreateInfoBuilder& infoBuilder) const {
  return Sampler(device, infoBuilder);
}
