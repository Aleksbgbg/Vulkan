#include "App.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <unordered_set>

#include "TexturedVertex.h"
#include "game/rendering/resources/MeshLoader.h"
#include "general/files/file.h"
#include "general/files/images/bmp.h"
#include "general/files/images/png.h"
#include "general/files/obj.h"
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

App::App()
    : window(1920, 1080),
      keyboard(window.GetKeyboard()),
      mouse(window.GetMouse()),
      hasSwapchain(false),
      hasOldSwapchain(false),
      previousTime(std::chrono::high_resolution_clock::now()),
      threadMessenger(),
      totalTime(0.0f) {
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

  const MeshLoader meshLoader(virtualDevice, deviceAllocator,
                              shortExecutionCommandBuffer, fence);

  player = Player(meshLoader.LoadMesh(MeshLoadParams{
      .frames = {MeshFrameLoadParams{.model =
                                         SPACESHIP_STATIONARY_MODEL_FILENAME},
                 MeshFrameLoadParams{.model = SPACESHIP_MOVING_MODEL_FILENAME}},
      .texture = SPACESHIP_TEXTURE_FILENAME,
      .emissive = SPACESHIP_EMISSIVE_FILENAME}));
  npc = Npc(meshLoader.LoadMesh(MeshLoadParams{
      .frames = {MeshFrameLoadParams{.model = NPC_SPACESHIP_MODEL_FILENAME}},
      .texture = NPC_SPACESHIP_TEXTURE_FILENAME,
      .emissive = NPC_SPACESHIP_EMISSIVE_FILENAME}));

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

  shaders.emplace_back(virtualDevice.LoadShader(
      VK_SHADER_STAGE_VERTEX_BIT, "shaders/textured_cube.vert.spv"));
  shaders.emplace_back(virtualDevice.LoadShader(
      VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/textured_cube.frag.spv"));

  const LightingData lightingData{.position = glm::vec3(10.0f, 10.0f, 10.0f)};

  lightingBuffer = TransferDataToGpuLocalMemory(
      shortExecutionCommandBuffer, &lightingData, sizeof(lightingData),
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

  InitializeSwapchain();

  for (u32 inFlightImage = 0; inFlightImage < framesInFlight; ++inFlightImage) {
    imagesInFlightSynchronisation.emplace_back(std::move<InFlightImage>(
        {.acquireImageSemaphore = virtualDevice.CreateSemaphore()}));
  }
}

void App::InitializeSwapchain() {
  VkSurfaceCapabilitiesKHR surfaceCapabilities =
      windowSurface.GetCapabilities(targetPhysicalDevice);
  VkSurfaceFormatKHR surfaceFormat =
      SelectSwapSurfaceFormat(windowSurface.GetFormats(targetPhysicalDevice));

  if (hasSwapchain) {
    oldSwapchain = std::move(swapchain);
    hasOldSwapchain = true;

    swapchain = virtualDevice.CreateSwapchain(
        windowSurface, oldSwapchain,
        SwapchainCreateInfoBuilder()
            .SetMinImageCount(minSwapchainImages + 1)
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
            .SetMinImageCount(minSwapchainImages + 1)
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

  VkFormat depthStencilFormat = SelectDepthStencilFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT});

  const VkExtent2D swapchainExtent = swapchain.GetImageExtent();

  projectionTransform.projection =
      glm::perspective(glm::radians(45.0f),
                       static_cast<float>(swapchainExtent.width) /
                           static_cast<float>(swapchainExtent.height),
                       0.1f, 1000.0f);
  projectionTransform.projection[1][1] *= -1;

  const VkSampleCountFlagBits samples =
      SelectMsaaSamples(VK_SAMPLE_COUNT_16_BIT);

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

  const std::array<VkAttachmentDescription, 3> attachmentDescriptions{
      AttachmentDescriptionBuilder()
          .SetFormat(swapchain.GetImageFormat())
          .SetSamples(VK_SAMPLE_COUNT_1_BIT)
          .SetLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
          .SetStoreOp(VK_ATTACHMENT_STORE_OP_STORE)
          .SetStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
          .SetStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetFinalLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR),
      AttachmentDescriptionBuilder()
          .SetFormat(swapchain.GetImageFormat())
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
  swapchainFramebuffers = swapchain.GetFramebuffers(
      renderPass, {&multisamplingImageView, &depthStencilView});

  constexpr const std::array<VkVertexInputBindingDescription, 1>
      vertexBindingDescriptions{
          VertexInputBindingDescriptionBuilder()
              .SetBinding(0)
              .SetStride(sizeof(TexturedVertex))
              .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
      };
  const std::array<VkVertexInputAttributeDescription, 3>
      vertexAttributeDescriptions{
          VertexInputAttributeDescriptionBuilder()
              .SetBinding(0)
              .SetLocation(0)
              .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
              .SetOffset(offsetof(TexturedVertex, position)),
          VertexInputAttributeDescriptionBuilder()
              .SetBinding(0)
              .SetLocation(1)
              .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
              .SetOffset(offsetof(TexturedVertex, normal)),
          VertexInputAttributeDescriptionBuilder()
              .SetBinding(0)
              .SetLocation(2)
              .SetFormat(VK_FORMAT_R32G32_SFLOAT)
              .SetOffset(offsetof(TexturedVertex, textureCoordinate)),
      };
  constexpr const VkDescriptorSetLayoutBinding
      perSceneDescriptorSetLayoutBinding =
          DescriptorSetLayoutBindingBuilder()
              .SetBinding(0)
              .SetDescriptorCount(1)
              .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
              .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT);
  perSceneDescriptorSetLayout = virtualDevice.CreateDescriptorSetLayout(
      DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
          &perSceneDescriptorSetLayoutBinding));
  constexpr const VkDescriptorSetLayoutBinding
      perFrameDescriptorSetLayoutBinding =
          DescriptorSetLayoutBindingBuilder()
              .SetBinding(0)
              .SetDescriptorCount(1)
              .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC)
              .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT |
                             VK_SHADER_STAGE_FRAGMENT_BIT);
  perFrameDescriptorSetLayout = virtualDevice.CreateDescriptorSetLayout(
      DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
          &perFrameDescriptorSetLayoutBinding));
  constexpr const VkDescriptorSetLayoutBinding
      lightingDescriptorSetLayoutBinding =
          DescriptorSetLayoutBindingBuilder()
              .SetBinding(0)
              .SetDescriptorCount(1)
              .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
              .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT);
  lightingDescriptorSetLayout = virtualDevice.CreateDescriptorSetLayout(
      DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
          &lightingDescriptorSetLayoutBinding));
  constexpr const std::array<VkDescriptorSetLayoutBinding, 2>
      textureSamplerLayoutBindings = {
          DescriptorSetLayoutBindingBuilder()
              .SetBinding(0)
              .SetDescriptorCount(1)
              .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
              .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT),
          DescriptorSetLayoutBindingBuilder()
              .SetBinding(1)
              .SetDescriptorCount(1)
              .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
              .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT),
      };
  playerTextureSamplerDescriptorSetLayout =
      virtualDevice.CreateDescriptorSetLayout(
          DescriptorSetLayoutCreateInfoBuilder()
              .SetBindingCount(textureSamplerLayoutBindings.size())
              .SetPBindings(textureSamplerLayoutBindings.data()));
  npcTextureSamplerDescriptorSetLayout =
      virtualDevice.CreateDescriptorSetLayout(
          DescriptorSetLayoutCreateInfoBuilder()
              .SetBindingCount(textureSamplerLayoutBindings.size())
              .SetPBindings(textureSamplerLayoutBindings.data()));
  const std::vector<const DescriptorSetLayout*> descriptorSetLayouts = {
      &perSceneDescriptorSetLayout, &perFrameDescriptorSetLayout,
      &lightingDescriptorSetLayout, &playerTextureSamplerDescriptorSetLayout,
      &npcTextureSamplerDescriptorSetLayout};
  const std::array<VkPushConstantRange, 1> pushConstantsRanges = {
      PushConstantRangeBuilder()
          .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT)
          .SetOffset(0)
          .SetSize(sizeof(PerObjectData))};
  pipeline = virtualDevice.CreateGraphicsPipeline(
      pipelineCache, shaders,
      virtualDevice.CreatePipelineLayout(
          descriptorSetLayouts,
          PipelineLayoutCreateInfoBuilder()
              .SetPushConstantRangeCount(pushConstantsRanges.size())
              .SetPPushConstantRanges(pushConstantsRanges.data())),
      SubpassReference(renderPass, 0),
      GraphicsPipelineCreateInfoBuilder()
          .SetPDepthStencilState(PipelineDepthStencilStateCreateInfoBuilder()
                                     .SetDepthTestEnable(VK_TRUE)
                                     .SetDepthWriteEnable(VK_TRUE)
                                     .SetDepthCompareOp(VK_COMPARE_OP_LESS)
                                     .SetDepthBoundsTestEnable(VK_FALSE))
          .SetPVertexInputState(PipelineVertexInputStateCreateInfoBuilder()
                                    .SetVertexBindingDescriptionCount(
                                        vertexBindingDescriptions.size())
                                    .SetPVertexBindingDescriptions(
                                        vertexBindingDescriptions.data())
                                    .SetVertexAttributeDescriptionCount(
                                        vertexAttributeDescriptions.size())
                                    .SetPVertexAttributeDescriptions(
                                        vertexAttributeDescriptions.data()))
          .SetPInputAssemblyState(
              PipelineInputAssemblyStateCreateInfoBuilder().SetTopology(
                  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST))
          .SetPViewportState(
              PipelineViewportStateCreateInfoBuilder()
                  .SetViewportCount(1)
                  .SetPViewports(
                      ViewportBuilder(VIEWPORT_BASE)
                          .SetWidth(static_cast<float>(swapchainExtent.width))
                          .SetHeight(
                              static_cast<float>(swapchainExtent.height)))
                  .SetScissorCount(1)
                  .SetPScissors(Rect2DBuilder()
                                    .SetOffset(OFFSET2D_ZERO)
                                    .SetExtent(swapchainExtent)))
          .SetPRasterizationState(
              PipelineRasterizationStateCreateInfoBuilder()
                  .SetCullMode(VK_CULL_MODE_BACK_BIT)
                  .SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE)
                  .SetPolygonMode(VK_POLYGON_MODE_FILL)
                  .SetLineWidth(1.0f))
          .SetPMultisampleState(PipelineMultisampleStateCreateInfoBuilder()
                                    .SetRasterizationSamples(samples)
                                    .SetMinSampleShading(1.0f))
          .SetPColorBlendState(
              PipelineColorBlendStateCreateInfoBuilder()
                  .SetAttachmentCount(1)
                  .SetPAttachments(
                      PipelineColorBlendAttachmentStateBuilder()
                          .SetColorWriteMask(VK_COLOR_COMPONENT_R_BIT |
                                             VK_COLOR_COMPONENT_G_BIT |
                                             VK_COLOR_COMPONENT_B_BIT |
                                             VK_COLOR_COMPONENT_A_BIT)
                          .SetSrcColorBlendFactor(VK_BLEND_FACTOR_ONE)
                          .SetDstColorBlendFactor(VK_BLEND_FACTOR_ZERO)
                          .SetColorBlendOp(VK_BLEND_OP_ADD)
                          .SetSrcAlphaBlendFactor(VK_BLEND_FACTOR_ONE)
                          .SetDstAlphaBlendFactor(VK_BLEND_FACTOR_ZERO)
                          .SetAlphaBlendOp(VK_BLEND_OP_ADD))));

  uiRenderer = nullptr;
  uiRenderer = std::make_unique<UiRenderer>(
      window.GetRect(),
      ImGuiInstance(window, instance, targetPhysicalDevice, virtualDevice,
                    queue, renderPass, shortExecutionCommandBuffer, fence,
                    samples));

  const u32 swapchainImages = swapchain.GetImageCount();
  constexpr const std::array<VkDescriptorPoolSize, 3> descriptorPoolSizes{
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetDescriptorCount(2),
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC)
          .SetDescriptorCount(1),
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetDescriptorCount(4),
  };
  descriptorPool = virtualDevice.CreateDescriptorPool(
      DescriptorPoolCreateInfoBuilder()
          .SetPoolSizeCount(descriptorPoolSizes.size())
          .SetPPoolSizes(descriptorPoolSizes.data())
          .SetMaxSets(5));
  sceneDescriptorSet =
      descriptorPool.AllocateDescriptorSet(perSceneDescriptorSetLayout);
  lightingDescriptorSet =
      descriptorPool.AllocateDescriptorSet(lightingDescriptorSetLayout);
  playerTextureSamplerDescriptorSet = descriptorPool.AllocateDescriptorSet(
      playerTextureSamplerDescriptorSetLayout);
  npcTextureSamplerDescriptorSet = descriptorPool.AllocateDescriptorSet(
      npcTextureSamplerDescriptorSetLayout);

  viewTransformBuffer = DynamicUniformBuffer<PerFrameData>(
      swapchainImages, physicalDeviceProperties.limits, descriptorPool,
      perFrameDescriptorSetLayout, virtualDevice, deviceAllocator);

  projectionTransformBuffer = TransferDataToGpuLocalMemory(
      shortExecutionCommandBuffer, &projectionTransform,
      sizeof(projectionTransform), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

  std::vector<std::unique_ptr<DescriptorSet::WriteDescriptorSet>>
      descriptorSetWrites;

  DescriptorSet::WriteDescriptorSet projectionBufferWrite;
  sceneDescriptorSet.CreateBufferWrite(
      projectionTransformBuffer.buffer, VK_WHOLE_SIZE,
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, projectionBufferWrite);
  descriptorSetWrites.emplace_back(
      std::make_unique<DescriptorSet::WriteDescriptorSet>(
          projectionBufferWrite));

  DescriptorSet::WriteDescriptorSet viewBufferWrite;
  viewTransformBuffer.CreateWriteDescriptorSet(viewBufferWrite);
  descriptorSetWrites.emplace_back(
      std::make_unique<DescriptorSet::WriteDescriptorSet>(viewBufferWrite));

  DescriptorSet::WriteDescriptorSet lightingBufferWrite;
  lightingDescriptorSet.CreateBufferWrite(lightingBuffer.buffer, VK_WHOLE_SIZE,
                                          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                          lightingBufferWrite);
  descriptorSetWrites.emplace_back(
      std::make_unique<DescriptorSet::WriteDescriptorSet>(lightingBufferWrite));

  {
    TextureRegistry textureRegistry(playerTextureSamplerDescriptorSet,
                                    textureSampler, descriptorSetWrites);
    player.WriteTexture(textureRegistry);
  }

  {
    TextureRegistry textureRegistry(npcTextureSamplerDescriptorSet,
                                    textureSampler, descriptorSetWrites);
    npc.WriteTexture(textureRegistry);
  }

  std::vector<VkWriteDescriptorSet> writeDescriptorSets(
      descriptorSetWrites.size());
  std::transform(descriptorSetWrites.begin(), descriptorSetWrites.end(),
                 writeDescriptorSets.begin(),
                 [](const std::unique_ptr<DescriptorSet::WriteDescriptorSet>&
                        writeDescriptorSet) { return *writeDescriptorSet; });

  virtualDevice.UpdateDescriptorSets(writeDescriptorSets.size(),
                                     writeDescriptorSets.data());

  for (u32 renderIndex = 0; renderIndex < swapchainImages; ++renderIndex) {
    swapchainRenderData.emplace_back(SwapchainRenderPass{
        .commandBuffer = renderCommandPool.AllocatePrimaryCommandBuffer(),
        .renderCompleteSemaphore = virtualDevice.CreateSemaphore(),
        .submitCompleteFence =
            virtualDevice.CreateFence(VK_FENCE_CREATE_SIGNALED_BIT)});
  }
  for (u32 renderIndex = 0; renderIndex < swapchainImages; ++renderIndex) {
    swapchainRenderData[renderIndex].meshRenderer = MeshRenderer(
        swapchainRenderData[renderIndex].commandBuffer, pipeline.GetLayout());
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

BufferWithMemory App::TransferDataToGpuLocalMemory(
    CommandBuffer& commandBuffer, const void* data, const u32 size,
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

  commandBuffer.BeginOneTimeSubmit();
  commandBuffer.CmdCopyBufferFull(stagingBuffer, finalBuffer);
  commandBuffer.End().Submit(fence).Wait().Reset();

  return {.buffer = std::move(finalBuffer),
          .memory = std::move(finalBufferMemory)};
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
  uiRenderer->ShowKeyboardLayout(window);

  const UpdateContext updateContext{.deltaTime = deltaTime,
                                    .keyboard = window.GetKeyboard()};

  player.UpdateModel(updateContext);
  npc.UpdateModel(updateContext);

  glm::vec3* const modelPosition = player.Position();

  glm::mat4 cameraTransform(1.0f);
  cameraTransform = glm::translate(cameraTransform, *modelPosition);

  const bool reverseView = keyboard.IsKeyDown(SDLK_c);

  glm::vec2 mouseDelta = mouse.GetPositionDeltaFromRightClick();

  if (reverseView) {
    mouseDelta.y = -mouseDelta.y;
  }

  const Rectf windowRect = window.GetRect();

  cameraTransform = glm::rotate(
      cameraTransform, glm::pi<float>() * (-mouseDelta.x / windowRect.width),
      glm::vec3(0.0f, 1.0f, 0.0f));
  cameraTransform = glm::rotate(
      cameraTransform,
      glm::pi<float>() *
          std::max(-0.499f, std::min(mouseDelta.y / windowRect.height, 0.499f)),
      glm::vec3(1.0f, 0.0f, 0.0f));

  if (reverseView) {
    cameraTransform = glm::rotate(cameraTransform, glm::pi<float>(),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
  }

  const glm::vec3 cameraLookAt = *modelPosition;
  const glm::vec3 cameraPosition =
      cameraTransform * glm::vec4(0.0f, 0.0f, -player.Size().z, 1.0f);

  totalTime += deltaTime;

  PerFrameData& frame = viewTransformBuffer.Value();
  frame.view =
      glm::lookAt(cameraPosition, cameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f));
  frame.cameraPosition = cameraPosition;
  frame.lightingPosition = glm::vec3(0.0f, 0.0f, 100000.0f);
  frame.material = {.ambient = glm::vec3(1.0f),   // , 0.5f, 0.31f),
                    .diffuse = glm::vec3(1.0f),   // , 0.5f, 0.31f),
                    .specular = glm::vec3(1.0f),  //, 0.5f, 0.5f),
                    .shininess = 32.0f};
  const glm::vec3 lightColor(77.0f / 255.0f, 77.0f / 255.0f, 1.0f);
  frame.light = {.position = glm::vec3(0.0f, 0.0f, 10000.0f),
                 .ambient = lightColor * 0.02f,
                 .diffuse = lightColor,
                 .specular = lightColor};

  // std::cout << glm::vec3(glm::vec4(*modelPosition, 1.0f) * frame.view) << std::endl;
  // std::cout << glm::vec3(frame.view * glm::vec4(frame.light.position, 1.0f)) << std::endl;

  uiRenderer->ShowObjectsInScene(
      ObjectsInSceneInfo{.cameraRotation = glm::vec2(0.0f, 0.0f),
                         .cameraLookAt = cameraLookAt,
                         .cameraPosition = cameraPosition,
                         .modelPosition = modelPosition});

  uiRenderer->EndFrame();
  window.EndFrame();
}

void App::Render() {
  InFlightImage& synchronisation =
      imagesInFlightSynchronisation[currentInFlightImage];

  const Swapchain::AcquireNextImageResult nextImageResult =
      swapchain.AcquireNextImage(SynchronisationPack().SetSignalSemaphore(
          &synchronisation.acquireImageSemaphore));
  const u32 imageIndex = nextImageResult.imageIndex;

  SwapchainRenderPass& swapchainRender = swapchainRenderData[imageIndex];

  swapchainRender.submitCompleteFence.Wait().Reset();
  {
    constexpr const VkClearValue colorClear =
        ClearValueBuilder().SetColor(ClearColorValueBuilder()
                                         .SetFloat0(0.0f)
                                         .SetFloat1(0.0f)
                                         .SetFloat2(0.0f)
                                         .SetFloat3(1.0f));
    constexpr const VkClearValue depthClear =
        ClearValueBuilder().SetDepthStencil(
            ClearDepthStencilValueBuilder().SetDepth(1.0f));

    constexpr const std::array<VkClearValue, 3> clearValues{
        colorClear,
        colorClear,
        depthClear,
    };

    viewTransformBuffer.Flush(imageIndex);

    swapchainRender.commandBuffer.Begin();
    swapchainRender.commandBuffer.CmdBeginRenderPass(
        RenderPassBeginInfoBuilder()
            .SetRenderArea(
                Rect2DBuilder().SetExtent(swapchain.GetImageExtent()))
            .SetClearValueCount(clearValues.size())
            .SetPClearValues(clearValues.data()),
        VK_SUBPASS_CONTENTS_INLINE, renderPass,
        swapchainFramebuffers[imageIndex]);
    swapchainRender.commandBuffer.CmdBindPipeline(
        VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    swapchainRender.commandBuffer.CmdBindDescriptorSets(
        VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetLayout(), 0, 1,
        sceneDescriptorSet);
    viewTransformBuffer.BindDescriptorSets(swapchainRender.commandBuffer,
                                           pipeline.GetLayout(), 1, imageIndex);
    swapchainRender.commandBuffer.CmdBindDescriptorSets(
        VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetLayout(), 2, 1,
        lightingDescriptorSet);

    swapchainRender.commandBuffer.CmdBindDescriptorSets(
        VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetLayout(), 3, 1,
        playerTextureSamplerDescriptorSet);
    player.Render(swapchainRender.meshRenderer);
    swapchainRender.commandBuffer.CmdBindDescriptorSets(
        VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetLayout(), 3, 1,
        npcTextureSamplerDescriptorSet);
    npc.Render(swapchainRender.meshRenderer);

    uiRenderer->Render(swapchainRender.commandBuffer);
    swapchainRender.commandBuffer.CmdEndRenderPass();
    swapchainRender.commandBuffer.End();
  }

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
