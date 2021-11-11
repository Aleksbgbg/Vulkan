#include "App.h"

#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <thread>

#include "general/files/file.h"
#include "general/logging/log.h"
#include "util/build_definition.h"
#include "util/filenames.h"
#include "vulkan/Buffer.h"
#include "vulkan/DescriptorPool.h"
#include "vulkan/structures/AttachmentReference.h"
#include "vulkan/structures/ClearValue.h"
#include "vulkan/structures/DescriptorSetLayoutBinding.h"
#include "vulkan/structures/DeviceQueueCreateInfo.h"
#include "vulkan/structures/PushConstantRange.h"
#include "vulkan/structures/SubpassDescription.h"
#include "vulkan/structures/default.h"
#include "vulkan/util.h"

static constexpr const u32 SWAPCHAIN_IMAGES = 3;

App::App()
    : window(1920, 1080),
      keyboard(window.GetKeyboard()),
      mouse(window.GetMouse()),
      hasSwapchain(false),
      hasOldSwapchain(false),
      previousTime(std::chrono::high_resolution_clock::now()),
      threadMessenger(),
      imageIndex(0) {
  const std::vector<VkExtensionProperties> availableExtensions =
      LoadArray(VulkanInstance::LoadInstanceExtensionProperties);

  const std::vector<const char*> requiredExtensions = {
      VK_KHR_SURFACE_EXTENSION_NAME,
      VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#ifdef VALIDATION
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

  instance = VulkanInstance(
      InstanceCreateInfoBuilder()
          .SetApplicationInfo(ApplicationInfoBuilder()
                                  .SetPApplicationName("Application")
                                  .SetApplicationVersion(1))
          .SetEnabledExtensionCount(requiredExtensions.size())
          .SetPpEnabledExtensionNames(requiredExtensions.data())
          .SetEnabledLayerCount(requiredLayers.size())
          .SetPpEnabledLayerNames(requiredLayers.data()));

#ifdef VALIDATION
  debugMessenger = instance.CreateDebugUtilsMessenger(
      DebugUtilsMessengerCreateInfoExtBuilder()
          .SetMessageSeverity(VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
          .SetMessageType(VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                          VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                          VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
          .SetPfnUserCallback(DebugCallback));
#endif

  windowSurface = window.CreateWindowSurface(instance);

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

  targetPhysicalDevice = physicalDevices[0];
  physicalDeviceProperties = targetPhysicalDevice.GetProperties();

  VkSurfaceCapabilitiesKHR surfaceCapabilities =
      windowSurface.GetCapabilities(targetPhysicalDevice);
  minSwapchainImages = std::min(surfaceCapabilities.minImageCount + 1,
                                surfaceCapabilities.maxImageCount);
  framesInFlight = minSwapchainImages;

  const std::optional<u32> queueFamilyIndex =
      targetPhysicalDevice.FindAppropriateQueueFamily(
          VK_QUEUE_GRAPHICS_BIT, [this](const u32 queueFamilyIndex) {
            return windowSurface.IsSupportedByPhysicalDevice(
                targetPhysicalDevice, queueFamilyIndex);
          });

  if (!queueFamilyIndex.has_value()) {
    throw std::runtime_error(
        "Could not find device queue family with required features.");
  }

  const std::vector<const char*> deviceExtensions{
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  virtualDevice = targetPhysicalDevice.CreateVirtualDevice(
      DeviceCreateInfoBuilder()
          .SetQueueCreateInfoCount(1)
          .SetPQueueCreateInfos(
              DeviceQueueCreateInfoBuilder()
                  .SetQueueCount(1)
                  .SetQueueFamilyIndex(queueFamilyIndex.value())
                  .SetPQueuePriorities(std::vector<float>{1.0f}.data())
                  .Build())
          .SetPEnabledFeatures(PhysicalDeviceFeaturesBuilder()
                                   .SetGeometryShader(VK_TRUE)
                                   .SetSamplerAnisotropy(VK_TRUE))
          .SetEnabledExtensionCount(deviceExtensions.size())
          .SetPpEnabledExtensionNames(deviceExtensions.data()));
  queue = virtualDevice.GetQueue(queueFamilyIndex.value(), 0);

  if (std::filesystem::exists(PIPELINE_CACHE_FILENAME)) {
    pipelineCache = virtualDevice.LoadPipelineCache(
        file::ReadFile(PIPELINE_CACHE_FILENAME));
  } else {
    pipelineCache = virtualDevice.CreatePipelineCache();
  }

  deviceAllocator = DeviceMemoryAllocator(
      &virtualDevice, targetPhysicalDevice.GetMemoryProperties());

  fence = virtualDevice.CreateFence();

  shortExecutionCommandPool =
      queue.CreateCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
  shortExecutionCommandBuffer =
      shortExecutionCommandPool.AllocatePrimaryCommandBuffer();

  textureSampler = virtualDevice.CreateSampler(
      SamplerCreateInfoBuilder()
          .SetMagFilter(VK_FILTER_LINEAR)
          .SetMinFilter(VK_FILTER_LINEAR)
          .SetAddressModeU(VK_SAMPLER_ADDRESS_MODE_REPEAT)
          .SetAddressModeV(VK_SAMPLER_ADDRESS_MODE_REPEAT)
          .SetAddressModeW(VK_SAMPLER_ADDRESS_MODE_REPEAT)
          .SetAnisotropyEnable(VK_TRUE)
          .SetMaxAnisotropy(
              physicalDeviceProperties.limits.maxSamplerAnisotropy));

  renderCommandPool =
      queue.CreateCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

  surfaceFormat =
      SelectSwapSurfaceFormat(windowSurface.GetFormats(targetPhysicalDevice));
  depthStencilFormat = SelectDepthStencilFormat({VK_FORMAT_D32_SFLOAT,
                                                 VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                 VK_FORMAT_D24_UNORM_S8_UINT});

  const VkFormat swapchainImageFormat = surfaceFormat.format;

  samples = SelectMsaaSamples(VK_SAMPLE_COUNT_16_BIT);

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
  renderPass = virtualDevice.CreateRenderPass(
      RenderPassCreateInfoBuilder()
          .SetAttachmentCount(attachmentDescriptions.size())
          .SetPAttachments(attachmentDescriptions.data())
          .SetSubpassCount(subpasses.size())
          .SetPSubpasses(subpasses.data())
          .SetDependencyCount(subpassDependencies.size())
          .SetPDependencies(subpassDependencies.data()));

  const SubpassReference subpass0 = SubpassReference(renderPass, 0);
  const VulkanContext vulkanContext{.virtualDevice = &virtualDevice,
                                    .pipelineCache = &pipelineCache,
                                    .subpassReference = &subpass0,
                                    .sampler = &textureSampler,
                                    .samples = samples};
  resourceAllocator = ResourceAllocator(virtualDevice, deviceAllocator,
                                        shortExecutionCommandBuffer, fence);
  const DynamicUniformBufferInitializer uniformBufferInitializer(
      SWAPCHAIN_IMAGES, physicalDeviceProperties.limits, virtualDevice,
      deviceAllocator);
  scene = std::make_unique<Scene>(vulkanContext, resourceAllocator, window,
                                  imageIndex, uniformBufferInitializer);

  InitializeSwapchain();

  for (u32 inFlightImage = 0; inFlightImage < framesInFlight; ++inFlightImage) {
    imagesInFlightSynchronisation.emplace_back(std::move<InFlightImage>(
        {.acquireImageSemaphore = virtualDevice.CreateSemaphore()}));
  }
}

void App::InitializeSwapchain() {
  VkSurfaceCapabilitiesKHR surfaceCapabilities =
      windowSurface.GetCapabilities(targetPhysicalDevice);

  const u32 swapchainImages = std::max(SWAPCHAIN_IMAGES, minSwapchainImages);

  if (hasSwapchain) {
    oldSwapchain = std::move(swapchain);
    hasOldSwapchain = true;

    swapchain = virtualDevice.CreateSwapchain(
        windowSurface, oldSwapchain,
        SwapchainCreateInfoBuilder()
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
            .SetClipped(VK_TRUE));
    hasSwapchain = true;
  } else {
    swapchain = virtualDevice.CreateSwapchain(
        windowSurface,
        SwapchainCreateInfoBuilder()
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
            .SetClipped(VK_TRUE));
    hasSwapchain = true;
  }

  if (swapchain.GetImageCount() != SWAPCHAIN_IMAGES) {
    // TODO: re-initialize the scene uniform if the number of images changes
    throw std::runtime_error(
        "Swapchain produced more images than the minimum.");
  }

  const VkExtent2D swapchainExtent = swapchain.GetImageExtent();

  Image depthStencilImage = virtualDevice.CreateImage(
      ImageCreateInfoBuilder(IMAGE_2D)
          .SetFormat(depthStencilFormat)
          .SetSamples(samples)
          .SetExtent(Extent3DBuilder(swapchainExtent).SetDepth(1))
          .SetUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
  DeviceMemorySubAllocation depthStencilMemory = deviceAllocator.BindMemory(
      depthStencilImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  depthStencilView = depthStencilImage.CreateView(
      ImageViewCreateInfoBuilder()
          .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
          .SetSubresourceRange(ImageSubresourceRangeBuilder()
                                   .SetAspectMask(VK_IMAGE_ASPECT_DEPTH_BIT)
                                   .SetLevelCount(1)
                                   .SetLayerCount(1)));

  depthStencil = {.image = std::move(depthStencilImage),
                  .memory = std::move(depthStencilMemory)};

  Image multisampling = virtualDevice.CreateImage(
      ImageCreateInfoBuilder(IMAGE_2D)
          .SetFormat(swapchain.GetImageFormat())
          .SetExtent(Extent3DBuilder(swapchainExtent).SetDepth(1))
          .SetSamples(samples)
          .SetUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT));
  DeviceMemorySubAllocation multisamplingMemory = deviceAllocator.BindMemory(
      multisampling, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  multisamplingImageView = multisampling.CreateView(
      ImageViewCreateInfoBuilder()
          .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
          .SetSubresourceRange(ImageSubresourceRangeBuilder()
                                   .SetAspectMask(VK_IMAGE_ASPECT_COLOR_BIT)
                                   .SetLevelCount(1)
                                   .SetLayerCount(1)));
  multisamplingImage = {.image = std::move(multisampling),
                        .memory = std::move(multisamplingMemory)};

  swapchainFramebuffers = swapchain.GetFramebuffers(
      renderPass, {&multisamplingImageView, &depthStencilView});

  uiRenderer = nullptr;
  uiRenderer = std::make_unique<UiRenderer>(
      window.GetRect(),
      ImGuiInstance(window, instance, targetPhysicalDevice, virtualDevice,
                    queue, renderPass, shortExecutionCommandBuffer, fence,
                    samples));

  scene->UpdateAspect(static_cast<float>(swapchainExtent.width) /
                      static_cast<float>(swapchainExtent.height));

  for (u32 renderIndex = 0; renderIndex < swapchain.GetImageCount();
       ++renderIndex) {
    swapchainRenderData.emplace_back(SwapchainRenderPass{
        .commandBuffer = renderCommandPool.AllocatePrimaryCommandBuffer(),
        .renderCompleteSemaphore = virtualDevice.CreateSemaphore(),
        .submitCompleteFence =
            virtualDevice.CreateFence(VK_FENCE_CREATE_SIGNALED_BIT)});
  }
}

App::~App() {
  virtualDevice.WaitIdle();
}

VkBool32 App::DebugCallback(
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

int App::Run() {
  std::thread renderThread(&App::RenderThread, this);
  MainThread();
  renderThread.join();
  file::WriteFile(PIPELINE_CACHE_FILENAME,
                  pipelineCache.GetPipelineCacheData());
  return 0;
}

void App::MainThread() {
  while (true) {
    switch (window.WaitAndProcessEvent()) {
      case Window::Event::Exit:
        threadMessenger.PostMessage(EventNotification::Unpaused);
        threadMessenger.PostMessage(EventNotification::Exited);
        return;

      case Window::Event::Minimized:
        threadMessenger.PostMessage(EventNotification::Paused);
        break;

      case Window::Event::Restored:
        threadMessenger.PostMessage(EventNotification::Unpaused);
        break;

      case Window::Event::SizeChanged:
        threadMessenger.PostMessage(EventNotification::Resized);
        break;
    }
  }
}

void App::RenderThread() {
  try {
    while (true) {
      MainLoop();

      while (threadMessenger.HasMessage()) {
        EventNotification message = threadMessenger.PopMessage();

        switch (message) {
          case EventNotification::Exited:
            return;

          case EventNotification::Paused:
            threadMessenger.WaitMessage(EventNotification::Unpaused);
            break;

          case EventNotification::Resized:
            // TODO: Allow continuous render during recreation
            virtualDevice.WaitIdle();
            swapchainRenderData.clear();
            BufferedLog("Recreating swapchain.");
            InitializeSwapchain();
            ImmediateLog("Swapchain recreated.");
            break;
        }
      }
    }
  } catch (const std::exception& exception) {
    ImmediateLog(exception.what());
    throw;
  }
}

void App::MainLoop() {
  const auto timeNow = std::chrono::high_resolution_clock::now();
  const float deltaTime =
      std::chrono::duration<float, std::chrono::seconds::period>(timeNow -
                                                                 previousTime)
          .count();

  UpdateModel(deltaTime);
  Render();

  previousTime = timeNow;
}

void App::UpdateModel(const float deltaTime) {
  uiRenderer->BeginFrame();
  uiRenderer->ShowVulkanDebugInfo(VulkanDebugInfo{
      .gpuName = physicalDeviceProperties.deviceName, .frametime = deltaTime});

  const UpdateContext updateContext{.deltaTime = deltaTime,
                                    .keyboard = window.GetKeyboard()};
  scene->UpdateModel(updateContext);

  uiRenderer->EndFrame();
  window.EndFrame();
}

void App::Render() {
  InFlightImage& synchronisation =
      imagesInFlightSynchronisation[currentInFlightImage];

  const Swapchain::AcquireNextImageResult nextImageResult =
      swapchain.AcquireNextImage(SynchronisationPack().SetSignalSemaphore(
          &synchronisation.acquireImageSemaphore));
  imageIndex = nextImageResult.imageIndex;

  SwapchainRenderPass& swapchainRender = swapchainRenderData[imageIndex];

  swapchainRender.submitCompleteFence.Wait().Reset();
  constexpr const VkClearValue colorClear =
      ClearValueBuilder().SetColor(ClearColorValueBuilder()
                                       .SetFloat0(0.0f)
                                       .SetFloat1(0.0f)
                                       .SetFloat2(0.0f)
                                       .SetFloat3(1.0f));
  constexpr const VkClearValue depthClear = ClearValueBuilder().SetDepthStencil(
      ClearDepthStencilValueBuilder().SetDepth(1.0f));

  constexpr const std::array<VkClearValue, 3> clearValues{
      colorClear,
      colorClear,
      depthClear,
  };

  swapchainRender.commandBuffer.Begin();
  swapchainRender.commandBuffer.CmdBeginRenderPass(
      RenderPassBeginInfoBuilder()
          .SetRenderArea(Rect2DBuilder().SetExtent(swapchain.GetImageExtent()))
          .SetClearValueCount(clearValues.size())
          .SetPClearValues(clearValues.data()),
      VK_SUBPASS_CONTENTS_INLINE, renderPass,
      swapchainFramebuffers[imageIndex]);
  scene->Render(swapchainRender.commandBuffer);
  uiRenderer->Render(swapchainRender.commandBuffer);
  swapchainRender.commandBuffer.CmdEndRenderPass();
  swapchainRender.commandBuffer.End();
  swapchainRender.commandBuffer.Submit(
      SynchronisationPack()
          .SetWaitSemaphore(&synchronisation.acquireImageSemaphore)
          .SetSignalSemaphore(&swapchainRender.renderCompleteSemaphore)
          .SetSignalFence(&swapchainRender.submitCompleteFence));
  queue.Present(swapchain, imageIndex,
                SynchronisationPack().SetWaitSemaphore(
                    &swapchainRender.renderCompleteSemaphore));

  currentInFlightImage = (currentInFlightImage + 1) % framesInFlight;
}

VkSurfaceFormatKHR App::SelectSwapSurfaceFormat(
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

VkExtent2D App::SelectSwapExtent(
    const VkSurfaceCapabilitiesKHR surfaceCapabilities) {
  // TODO: Check the framebuffer size if the currentExtent is a special value
  // UINT32_MAX meaning the picture can be a different size than the window
  return surfaceCapabilities.currentExtent;
}

VkPresentModeKHR App::SelectSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes) {
  BufferedLog("Selecting vertically synced swapchain present mode.");
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkFormat App::SelectDepthStencilFormat(
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

VkSampleCountFlagBits App::SelectMsaaSamples(
    const VkSampleCountFlagBits preferred) {
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
