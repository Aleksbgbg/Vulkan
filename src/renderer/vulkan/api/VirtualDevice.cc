#include "VirtualDevice.h"

#include <algorithm>

#include "Surface.h"
#include "error.h"
#include "general/files/file.h"

namespace vk {

VirtualDevice::VirtualDevice() : device(nullptr) {}

VirtualDevice::VirtualDevice(VkPhysicalDevice physicalDevice,
                             const DeviceCreateInfoBuilder& infoBuilder) {
  PROCEED_ON_VALID_RESULT(
      vkCreateDevice(physicalDevice, infoBuilder.Build(), nullptr, &device));
}

VirtualDevice::VirtualDevice(VirtualDevice&& other) noexcept
    : device(other.device) {
  other.device = nullptr;
}

VirtualDevice::~VirtualDevice() {
  if (device != nullptr) {
    vkDestroyDevice(device, nullptr);
  }
}

VirtualDevice& VirtualDevice::operator=(VirtualDevice&& other) noexcept {
  this->~VirtualDevice();
  new (this) VirtualDevice(std::move(other));
  return *this;
}

Queue VirtualDevice::GetQueue(const u32 familyIndex,
                              const u32 queueIndex) const {
  VkQueue queue;
  vkGetDeviceQueue(device, familyIndex, queueIndex, &queue);
  return Queue(device, queue, familyIndex);
}

Buffer VirtualDevice::CreateBuffer(BufferCreateInfoBuilder& infoBuilder) const {
  return Buffer(device, infoBuilder);
}

Image VirtualDevice::CreateImage(ImageCreateInfoBuilder& infoBuilder) const {
  return Image(device, infoBuilder);
}

Swapchain VirtualDevice::CreateSwapchain(
    const Surface& surface, SwapchainCreateInfoBuilder infoBuilder) const {
  return Swapchain(device, infoBuilder.SetSurface(surface.surface));
}

Swapchain VirtualDevice::CreateSwapchain(
    const Surface& surface, const Swapchain& oldSwapchain,
    SwapchainCreateInfoBuilder infoBuilder) const {
  return Swapchain(device, oldSwapchain,
                   infoBuilder.SetSurface(surface.surface));
}

ShaderModule VirtualDevice::LoadComputeShader(
    const std::string_view shaderFilename) const {
  return LoadShader(VK_SHADER_STAGE_COMPUTE_BIT, shaderFilename);
}

ShaderModule VirtualDevice::LoadShader(
    const VkShaderStageFlagBits stage,
    const std::string_view shaderFilename) const {
  const std::vector<u8> code = file::ReadFile(shaderFilename);
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
    const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const {
  return DescriptorSetLayout(device, infoBuilder);
}

PipelineLayout VirtualDevice::CreatePipelineLayout(
    const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
    const PipelineLayoutCreateInfoBuilder& infoBuilder) const {
  return PipelineLayout(device, infoBuilder, descriptorSetLayouts);
}

RenderPass VirtualDevice::CreateRenderPass(
    RenderPassCreateInfoBuilder& infoBuilder) const {
  return RenderPass(device, infoBuilder);
}

GraphicsPipeline VirtualDevice::CreateGraphicsPipeline(
    const PipelineCache& pipelineCache,
    const std::vector<ShaderModule>& shaders, PipelineLayout pipelineLayout,
    const SubpassReference subpassReference,
    const GraphicsPipelineCreateInfoBuilder& infoBuilder) const {
  return GraphicsPipeline(device, pipelineCache, shaders,
                          std::move(pipelineLayout), subpassReference,
                          infoBuilder);
}

Semaphore VirtualDevice::CreateSemaphore() const {
  return Semaphore(device, SemaphoreCreateInfoBuilder().Reference());
}

ComputePipeline VirtualDevice::CreateComputePipeline(
    const PipelineCache& pipelineCache, PipelineLayout pipelineLayout,
    const ShaderModule& shaderModule) const {
  return ComputePipeline(device, pipelineCache, std::move(pipelineLayout),
                         shaderModule);
}

void VirtualDevice::UpdateDescriptorSets(
    const u32 count, const VkWriteDescriptorSet* const writes) const {
  vkUpdateDescriptorSets(device, count, writes, 0, nullptr);
}

void VirtualDevice::UpdateDescriptorSets(
    const DescriptorSet::WriteDescriptorSet& descriptorSetWrite) const {
  const VkWriteDescriptorSet writeDescriptorSet = descriptorSetWrite.Build();
  vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
}

void VirtualDevice::UpdateDescriptorSets(
    const std::vector<DescriptorSet::WriteDescriptorSet>& descriptorSetWrites)
    const {
  std::vector<VkWriteDescriptorSet> rawWrites(descriptorSetWrites.size());
  std::transform(descriptorSetWrites.begin(), descriptorSetWrites.end(),
                 rawWrites.begin(),
                 [](const DescriptorSet::WriteDescriptorSet& write) {
                   return write.Build();
                 });
  vkUpdateDescriptorSets(device, rawWrites.size(), rawWrites.data(), 0,
                         nullptr);
}

DescriptorPool VirtualDevice::CreateDescriptorPool(
    const DescriptorPoolCreateInfoBuilder& infoBuilder) const {
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

PipelineCache VirtualDevice::CreatePipelineCache() const {
  return PipelineCache(device, PipelineCacheCreateInfoBuilder().Reference());
}

PipelineCache VirtualDevice::LoadPipelineCache(
    const std::vector<u8>& data) const {
  return PipelineCache(device, PipelineCacheCreateInfoBuilder()
                                   .SetInitialDataSize(data.size())
                                   .SetPInitialData(data.data()));
}

}  // namespace vk
