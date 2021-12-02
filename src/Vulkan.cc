#include "Vulkan.h"

#include <array>
#include <filesystem>
#include <stdexcept>

#include "general/files/file.h"
#include "general/files/images/png.h"
#include "general/logging/log.h"
#include "general/math/math.h"
#include "memory/Alignment.h"
#include "util/build_definition.h"
#include "util/filenames.h"
#include "vulkan/Buffer.h"
#include "vulkan/DescriptorPool.h"
#include "vulkan/VulkanInstance.h"
#include "vulkan/structures/AttachmentReference.h"
#include "vulkan/structures/ClearValue.h"
#include "vulkan/structures/DescriptorSetLayoutBinding.h"
#include "vulkan/structures/DeviceQueueCreateInfo.h"
#include "vulkan/structures/PushConstantRange.h"
#include "vulkan/structures/SubpassDescription.h"
#include "vulkan/structures/default.h"
#include "vulkan/util.h"

static constexpr u32 WANTED_SWAPCHAIN_IMAGES = 3u;

VulkanInstance CreateVulkanInstance(const VulkanSystem& vulkanSystem) {
  const std::vector<VkExtensionProperties> availableExtensions =
      LoadArray(VulkanInstance::LoadInstanceExtensionProperties);

  const std::vector<const char*> requiredExtensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    vulkanSystem.SurfaceExtensionName(),
#if defined(VALIDATION)
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
  };

  if (!RequiredValuesAreAvailable(availableExtensions, requiredExtensions,
                                  [](const VkExtensionProperties& property) {
                                    return property.extensionName;
                                  })) {
    throw std::runtime_error("Required extensions not available.");
  }

  const std::vector<VkLayerProperties> availableLayers =
      LoadArray(VulkanInstance::LoadInstanceLayerProperties);
  const std::vector<const char*> requiredLayers = {
#ifdef VALIDATION
      "VK_LAYER_KHRONOS_validation",
#endif
  };

  if (!RequiredValuesAreAvailable(availableLayers, requiredLayers,
                                  [](const VkLayerProperties& property) {
                                    return property.layerName;
                                  })) {
    throw std::runtime_error("Required layers not available.");
  }

  return VulkanInstance(
      InstanceCreateInfoBuilder()
          .SetApplicationInfo(ApplicationInfoBuilder()
                                  .SetPApplicationName("Application")
                                  .SetApplicationVersion(1))
          .SetEnabledExtensionCount(requiredExtensions.size())
          .SetPpEnabledExtensionNames(requiredExtensions.data())
          .SetEnabledLayerCount(requiredLayers.size())
          .SetPpEnabledLayerNames(requiredLayers.data()));
}

PhysicalDevice ChoosePhysicalDevice(const VulkanInstance& instance) {
  std::vector<PhysicalDevice> physicalDevices = instance.GetPhysicalDevices();

  if (physicalDevices.empty()) {
    throw std::runtime_error("No Vulkan-enabled GPUs found on the machine.");
  }

  BufferedLog(std::string("Found ") + std::to_string(physicalDevices.size()) +
              " GPU(s):");
  for (int index = 0; index < physicalDevices.size(); ++index) {
    BufferedLog(std::string("[") + std::to_string(index) + "] " +
                physicalDevices[index].GetProperties().deviceName);
  }
  ImmediateLog("Selecting GPU 0 as render target.");

  return physicalDevices[0];
}

u32 CalculateSwapchainImages(
    const VkSurfaceCapabilitiesKHR& surfaceCapabilities) {
  const u32 maxImageCount = surfaceCapabilities.maxImageCount == 0
                                ? WANTED_SWAPCHAIN_IMAGES
                                : surfaceCapabilities.maxImageCount;

  return CoerceToRange(WANTED_SWAPCHAIN_IMAGES,
                       surfaceCapabilities.minImageCount, maxImageCount);
}

u32 ChooseQueueFamily(const PhysicalDevice& physicalDevice,
                      const Surface& windowSurface) {
  const std::optional<u32> queueFamilyIndex =
      physicalDevice.FindAppropriateQueueFamily(
          VK_QUEUE_GRAPHICS_BIT, [&](const u32 queueFamilyIndex) {
            return windowSurface.IsSupportedByPhysicalDevice(physicalDevice,
                                                             queueFamilyIndex);
          });

  if (!queueFamilyIndex.has_value()) {
    throw std::runtime_error(
        "Could not find device queue family with required features.");
  }

  return queueFamilyIndex.value();
}

VirtualDevice CreateVirtualDevice(const PhysicalDevice& physicalDevice,
                                  const u32 queueFamilyIndex) {
  const std::vector<const char*> deviceExtensions{
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  const float queuePriorities[] = {1.0f};
  return physicalDevice.CreateVirtualDevice(
      DeviceCreateInfoBuilder()
          .SetQueueCreateInfoCount(1)
          .SetPQueueCreateInfos(DeviceQueueCreateInfoBuilder()
                                    .SetQueueCount(1)
                                    .SetQueueFamilyIndex(queueFamilyIndex)
                                    .SetPQueuePriorities(queuePriorities)
                                    .Build())
          .SetPEnabledFeatures(PhysicalDeviceFeaturesBuilder()
                                   .SetGeometryShader(VK_TRUE)
                                   .SetSamplerAnisotropy(VK_TRUE))
          .SetEnabledExtensionCount(deviceExtensions.size())
          .SetPpEnabledExtensionNames(deviceExtensions.data()));
}

PipelineCache LoadOrCreatePipelineCache(const VirtualDevice& virtualDevice) {
  if (std::filesystem::exists(PIPELINE_CACHE_FILENAME)) {
    return virtualDevice.LoadPipelineCache(
        file::ReadFile(PIPELINE_CACHE_FILENAME));
  }

  return virtualDevice.CreatePipelineCache();
}

VkSurfaceFormatKHR Vulkan::SelectSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats) const {
  for (const auto& format : availableFormats) {
    if ((format.format == VK_FORMAT_B8G8R8A8_SRGB) &&
        (format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)) {
      BufferedLog("Selecting sRGB 32-bit BGRA swapchain format.");
      return format;
    }
  }

  BufferedLog(
      "Selecting first available swapchain format (could not find sRGB).");
  return availableFormats[0];
}

VkExtent2D Vulkan::SelectSwapExtent(
    const VkSurfaceCapabilitiesKHR surfaceCapabilities) {
  // TODO: Check the framebuffer size if the currentExtent is a special value
  // UINT32_MAX meaning the picture can be a different size than the window
  return surfaceCapabilities.currentExtent;
}

VkPresentModeKHR Vulkan::SelectSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes) {
  BufferedLog("Selecting vertically synced swapchain present mode.");
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkFormat Vulkan::SelectDepthStencilFormat(
    const std::vector<VkFormat>& potentialFormats) const {
  for (const VkFormat format : potentialFormats) {
    const VkFormatProperties formatProperties =
        targetPhysicalDevice.GetFormatProperties(format);

    if (formatProperties.optimalTilingFeatures &
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
      return format;
    }
  }

  throw std::runtime_error("Could not find suitable depth stencil format.");
}

VkSampleCountFlagBits Vulkan::SelectMsaaSamples(
    const VkSampleCountFlagBits preferred) const {
  const VkSampleCountFlags supportedSamples =
      physicalDeviceProperties.limits.framebufferColorSampleCounts &
      physicalDeviceProperties.limits.framebufferDepthSampleCounts;

  for (VkSampleCountFlagBits samples = preferred;
       samples > VK_SAMPLE_COUNT_1_BIT;
       samples = static_cast<VkSampleCountFlagBits>(samples >> 1)) {
    if ((supportedSamples & samples) == samples) {
      return samples;
    }
  }

  return VK_SAMPLE_COUNT_1_BIT;
}

RenderPass Vulkan::CreateRenderPass() const {
  const VkFormat swapchainImageFormat = surfaceFormat.format;
  const std::array<VkAttachmentDescription, 3> attachmentDescriptions{
      AttachmentDescriptionBuilder()
          .SetFormat(swapchainImageFormat)
          .SetSamples(VK_SAMPLE_COUNT_1_BIT)
          .SetLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
          .SetStoreOp(VK_ATTACHMENT_STORE_OP_STORE)
          .SetStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
          .SetStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetFinalLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR),
      AttachmentDescriptionBuilder()
          .SetFormat(swapchainImageFormat)
          .SetSamples(samples)
          .SetLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
          .SetStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
          .SetStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetFinalLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
      AttachmentDescriptionBuilder()
          .SetFormat(depthStencilFormat)
          .SetSamples(samples)
          .SetLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
          .SetStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
          .SetStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetFinalLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL),
  };
  constexpr const VkAttachmentReference resolveAttachment =
      AttachmentReferenceBuilder().SetAttachment(0).SetLayout(
          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  constexpr const VkAttachmentReference colorAttachment =
      AttachmentReferenceBuilder().SetAttachment(1).SetLayout(
          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  constexpr const VkAttachmentReference depthStencilAttachment =
      AttachmentReferenceBuilder().SetAttachment(2).SetLayout(
          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  const std::array<VkSubpassDescription, 1> subpasses{
      SubpassDescriptionBuilder()
          .SetPipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
          .SetPResolveAttachments(&resolveAttachment)
          .SetColorAttachmentCount(1)
          .SetPColorAttachments(&colorAttachment)
          .SetPDepthStencilAttachment(&depthStencilAttachment),
  };
  constexpr const std::array<VkSubpassDependency, 1> subpassDependencies{
      SubpassDependencyBuilder()
          .SetSrcSubpass(VK_SUBPASS_EXTERNAL)
          .SetDstSubpass(0)
          .SetSrcStageMask(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                           VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)
          .SetDstStageMask(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                           VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)
          .SetSrcAccessMask(0)
          .SetDstAccessMask(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT),
  };

  return virtualDevice.CreateRenderPass(
      RenderPassCreateInfoBuilder()
          .SetAttachmentCount(attachmentDescriptions.size())
          .SetPAttachments(attachmentDescriptions.data())
          .SetSubpassCount(subpasses.size())
          .SetPSubpasses(subpasses.data())
          .SetDependencyCount(subpassDependencies.size())
          .SetPDependencies(subpassDependencies.data()));
}

Vulkan::Vulkan(const VulkanSystem& vulkanSystem, const sys::Window& window)
    : instance(CreateVulkanInstance(vulkanSystem)),
#ifdef VALIDATION
      debugMessenger(instance.CreateDebugUtilsMessenger(
          DebugUtilsMessengerCreateInfoExtBuilder()
              .SetMessageSeverity(
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
              .SetMessageType(VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
              .SetPfnUserCallback(DebugCallback))),
#endif
      windowSurface(instance.CreateSurface(window)),
      targetPhysicalDevice(ChoosePhysicalDevice(instance)),
      physicalDeviceProperties(targetPhysicalDevice.GetProperties()),
      swapchainImages(CalculateSwapchainImages(
          windowSurface.GetCapabilities(targetPhysicalDevice))),
      queueFamilyIndex(ChooseQueueFamily(targetPhysicalDevice, windowSurface)),
      virtualDevice(
          CreateVirtualDevice(targetPhysicalDevice, queueFamilyIndex)),
      queue(virtualDevice.GetQueue(queueFamilyIndex, 0)),
      pipelineCache(LoadOrCreatePipelineCache(virtualDevice)),
      deviceAllocator(&virtualDevice,
                      targetPhysicalDevice.GetMemoryProperties()),
      fence(virtualDevice.CreateFence()),
      shortExecutionCommandPool(queue.CreateCommandPool(
          VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)),
      shortExecutionCommandBuffer(
          shortExecutionCommandPool.AllocatePrimaryCommandBuffer()),
      surfaceFormat(SelectSwapSurfaceFormat(
          windowSurface.GetFormats(targetPhysicalDevice))),
      depthStencilFormat(SelectDepthStencilFormat(
          {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
           VK_FORMAT_D24_UNORM_S8_UINT})),
      samples(SelectMsaaSamples(VK_SAMPLE_COUNT_16_BIT)),
      textureSampler(virtualDevice.CreateSampler(
          SamplerCreateInfoBuilder()
              .SetMagFilter(VK_FILTER_LINEAR)
              .SetMinFilter(VK_FILTER_LINEAR)
              .SetAddressModeU(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAddressModeV(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAddressModeW(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAnisotropyEnable(VK_TRUE)
              .SetMaxAnisotropy(
                  physicalDeviceProperties.limits.maxSamplerAnisotropy))),
      renderCommandPool(queue.CreateCommandPool(
          VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)),
      renderPass(CreateRenderPass()),
      subpass0(SubpassReference(renderPass, 0)) {
}

Vulkan::~Vulkan() {
  virtualDevice.WaitIdle();
  file::WriteFile(PIPELINE_CACHE_FILENAME,
                  pipelineCache.GetPipelineCacheData());
}

VkBool32 Vulkan::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
  if ((pCallbackData->messageIdNumber == 0xc036022f) ||
      (pCallbackData->messageIdNumber == 0x618ab1e7)) {
    return VK_FALSE;
  }

  ImmediateLog(pCallbackData->pMessage);
  return VK_FALSE;
}

CommandBuffer Vulkan::AllocatePrimaryRenderCommandBuffer() const {
  return renderCommandPool.AllocatePrimaryCommandBuffer();
}

Fence Vulkan::CreateFence(const VkFenceCreateFlags flags) const {
  return virtualDevice.CreateFence(flags);
}

void Vulkan::CmdBeginRenderPass(const CommandBuffer& commandBuffer,
                                const RenderPassBeginInfoBuilder& infoBuilder,
                                const Framebuffer& framebuffer) const {
  commandBuffer.CmdBeginRenderPass(infoBuilder, VK_SUBPASS_CONTENTS_INLINE,
                                   renderPass, framebuffer);
}

void Vulkan::Present(const SwapchainWithResources& swapchain,
                     const SynchronisationPack& synchronisation) const {
  swapchain.Present(queue, synchronisation);
}

void Vulkan::WaitIdle() const {
  virtualDevice.WaitIdle();
}

Texture Vulkan::LoadTexture(const std::string_view filename) {
  const file::Image image = file::ReadPng(filename);

  const Buffer stagingBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(image.size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  const DeviceMemorySubAllocation stagingBufferMemory =
      deviceAllocator.BindMemory(stagingBuffer,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(image.data.data(), stagingBuffer.Size());

  Image textureImage =
      virtualDevice.CreateImage(ImageCreateInfoBuilder(IMAGE_2D)
                                    .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
                                    .SetExtent(Extent3DBuilder()
                                                   .SetWidth(image.width)
                                                   .SetHeight(image.height)
                                                   .SetDepth(1))
                                    .SetUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                              VK_IMAGE_USAGE_SAMPLED_BIT));
  DeviceMemorySubAllocation textureImageMemory = deviceAllocator.BindMemory(
      textureImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  shortExecutionCommandBuffer.BeginOneTimeSubmit();
  shortExecutionCommandBuffer.CmdImageMemoryBarrier(
      textureImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(0)
          .SetDstAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetNewLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  shortExecutionCommandBuffer.CmdCopyBufferToImage(
      stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      BufferImageCopyBuilder()
          .SetImageSubresource(SUBRESOURCE_LAYERS_COLOR_SINGLE_LAYER)
          .SetImageExtent(Extent3DBuilder(EXTENT3D_SINGLE_DEPTH)
                              .SetWidth(image.width)
                              .SetHeight(image.height)));
  shortExecutionCommandBuffer.CmdImageMemoryBarrier(
      textureImage, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetDstAccessMask(VK_ACCESS_SHADER_READ_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetNewLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  shortExecutionCommandBuffer.End().Submit(fence).Wait().Reset();

  ImageView textureView = textureImage.CreateView(
      ImageViewCreateInfoBuilder()
          .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
          .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));

  return Texture{.image = {.image = std::move(textureImage),
                           .memory = std::move(textureImageMemory)},
                 .view = std::move(textureView)};
}

BufferWithMemory Vulkan::AllocateLocalBuffer(const std::size_t size,
                                             const VkBufferUsageFlags usage) {
  BufferWithMemory bufferWithMemory;
  bufferWithMemory.buffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE).SetSize(size).SetUsage(usage));
  bufferWithMemory.memory = deviceAllocator.BindMemory(
      bufferWithMemory.buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  return bufferWithMemory;
}

BufferWithMemory Vulkan::AllocateDeviceBuffer(const void* const data,
                                              const std::size_t size,
                                              const VkBufferUsageFlags usage) {
  Buffer stagingBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  DeviceMemorySubAllocation stagingBufferMemory = deviceAllocator.BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(data, stagingBuffer.Size());

  Buffer finalBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage));
  DeviceMemorySubAllocation finalBufferMemory = deviceAllocator.BindMemory(
      finalBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  shortExecutionCommandBuffer.BeginOneTimeSubmit();
  shortExecutionCommandBuffer.CmdCopyBufferFull(stagingBuffer, finalBuffer);
  shortExecutionCommandBuffer.End().Submit(fence).Wait().Reset();

  return {.buffer = std::move(finalBuffer),
          .memory = std::move(finalBufferMemory)};
}

SwapchainCreateInfoBuilder Vulkan::SwapchainCreateInfo() const {
  const VkSurfaceCapabilitiesKHR surfaceCapabilities =
      windowSurface.GetCapabilities(targetPhysicalDevice);
  return SwapchainCreateInfoBuilder()
      .SetMinImageCount(swapchainImages)
      .SetImageFormat(surfaceFormat.format)
      .SetImageColorSpace(surfaceFormat.colorSpace)
      .SetImageExtent(SelectSwapExtent(surfaceCapabilities))
      .SetImageArrayLayers(1)
      .SetImageUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
      .SetImageSharingMode(VK_SHARING_MODE_EXCLUSIVE)
      .SetPreTransform(surfaceCapabilities.currentTransform)
      .SetCompositeAlpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
      .SetPresentMode(SelectSwapPresentMode(
          windowSurface.GetPresentModes(targetPhysicalDevice)))
      .SetClipped(VK_TRUE);
}

Swapchain Vulkan::CreateSwapchain() const {
  return virtualDevice.CreateSwapchain(windowSurface, SwapchainCreateInfo());
}

Swapchain Vulkan::CreateSwapchain(const Swapchain& oldSwapchain) const {
  return virtualDevice.CreateSwapchain(windowSurface, oldSwapchain,
                                       SwapchainCreateInfo());
}

ImageWithMemory Vulkan::CreateDepthStencilAttachment(
    const Swapchain& swapchain) {
  ImageWithMemory depthStencil;
  depthStencil.image = virtualDevice.CreateImage(
      ImageCreateInfoBuilder(IMAGE_2D)
          .SetFormat(depthStencilFormat)
          .SetSamples(samples)
          .SetExtent(Extent3DBuilder(swapchain.GetImageExtent()).SetDepth(1))
          .SetUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
  depthStencil.memory = deviceAllocator.BindMemory(
      depthStencil.image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  return depthStencil;
}

ImageWithMemory Vulkan::CreateMultisamplingAttachment(
    const Swapchain& swapchain) {
  ImageWithMemory multisampling;
  multisampling.image = virtualDevice.CreateImage(
      ImageCreateInfoBuilder(IMAGE_2D)
          .SetFormat(swapchain.GetImageFormat())
          .SetExtent(Extent3DBuilder(swapchain.GetImageExtent()).SetDepth(1))
          .SetSamples(samples)
          .SetUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT));
  multisampling.memory = deviceAllocator.BindMemory(
      multisampling.image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  return multisampling;
}

std::vector<Framebuffer> Vulkan::GetFramebuffers(
    const Swapchain& swapchain,
    const std::vector<const ImageView*>& attachments) const {
  return swapchain.GetFramebuffers(renderPass, attachments);
}

Semaphore Vulkan::CreateSemaphore() const {
  return virtualDevice.CreateSemaphore();
}

u32 Vulkan::PaddedSize(const u32 elementSize) const {
  const u32 minimumSize = physicalDeviceProperties.limits.nonCoherentAtomSize;
  const u32 alignment =
      physicalDeviceProperties.limits.minUniformBufferOffsetAlignment;

  const float ratio =
      static_cast<float>(minimumSize) / static_cast<float>(alignment);
  const u32 requiredAlignment = alignment * static_cast<u32>(std::ceil(ratio));
  return Align(elementSize, requiredAlignment);
}

BufferWithMemory Vulkan::CreateBuffer(const u32 elementPaddedSize) {
  const u32 objectCount = swapchainImages;

  BufferWithMemory bufferWithMemory;
  bufferWithMemory.buffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(elementPaddedSize * objectCount)
          .SetUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT));
  bufferWithMemory.memory = deviceAllocator.BindMemory(
      bufferWithMemory.buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  return bufferWithMemory;
}

Pipeline Vulkan::CreateGraphicsPipeline(
    const std::vector<ShaderModule>& shaders,
    const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
    const PipelineLayoutCreateInfoBuilder& pipelineLayout,
    const GraphicsPipelineCreateInfoBuilder& infoBuilder) const {
  return virtualDevice.CreateGraphicsPipeline(
      pipelineCache, shaders,
      virtualDevice.CreatePipelineLayout(descriptorSetLayouts, pipelineLayout),
      subpass0, infoBuilder);
}

PipelineMultisampleStateCreateInfoBuilder Vulkan::CreateMultisampleState()
    const {
  return PipelineMultisampleStateCreateInfoBuilder().SetRasterizationSamples(
      samples);
}

DescriptorSet::WriteDescriptorSet Vulkan::CreateImageSamplerWrite(
    const DescriptorSet& descriptorSet, const ImageView& imageView,
    const u32 binding) const {
  return descriptorSet.CreateImageSamplerWrite(
      imageView, textureSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      binding);
}

ShaderModule Vulkan::LoadShader(const VkShaderStageFlagBits stage,
                                const std::string_view shaderFilename) const {
  return virtualDevice.LoadShader(stage, shaderFilename);
}

DescriptorSetLayout Vulkan::CreateDescriptorSetLayout(
    const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const {
  return virtualDevice.CreateDescriptorSetLayout(infoBuilder);
}

DescriptorPool Vulkan::CreateDescriptorPool(
    const DescriptorPoolCreateInfoBuilder& infoBuilder) const {
  return virtualDevice.CreateDescriptorPool(infoBuilder);
}

void Vulkan::UpdateDescriptorSets(
    const u32 descriptorSetCount,
    const VkWriteDescriptorSet* const descriptorSetWrites) const {
  virtualDevice.UpdateDescriptorSets(descriptorSetCount, descriptorSetWrites);
}
