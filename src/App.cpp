#include "App.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>

#include "GradientVertex.h"
#include "Model.h"
#include "VertexBuilder.h"
#include "build_definition.h"
#include "file.h"
#include "filenames.h"
#include "include_sdl.h"
#include "read_bitmap.h"
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

void Log(const std::string_view text) {
  std::cout << text << std::endl;
}

App::WindowInfo App::InitSdl() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  Recti windowRect(50, 50, 1920, 1080);

  SDL_Window* window = SDL_CreateWindow(
      "Vulkan", windowRect.x, windowRect.y, windowRect.width, windowRect.height,
      SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  SDL_SysWMinfo sysWmInfo;
  SDL_VERSION(&sysWmInfo.version)
  SDL_GetWindowWMInfo(window, &sysWmInfo);

  return WindowInfo{.window = window,
                    .hinstance = sysWmInfo.info.win.hinstance,
                    .hwnd = sysWmInfo.info.win.window,
                    .rect = windowRect};
}

namespace std {
template <>
class hash<TexturedVertex> {
 public:
  size_t operator()(const TexturedVertex& vertex) const {
    size_t xHash = std::hash<u32>()(vertex.position.x);
    size_t yHash = std::hash<u32>()(vertex.position.y);
    size_t zHash = std::hash<u32>()(vertex.position.z);
    size_t uHash = std::hash<u32>()(vertex.textureCoordinate.x);
    size_t vHash = std::hash<u32>()(vertex.textureCoordinate.y);
    return xHash ^ yHash ^ zHash ^ uHash ^ vHash;
  }
};
}  // namespace std

bool operator==(const TexturedVertex& left, const TexturedVertex& right) {
  return (left.position == right.position) &&
         (left.textureCoordinate == right.textureCoordinate);
}

App::App()
    : windowInfo(InitSdl()),
      hasSwapchain(false),
      hasOldSwapchain(false),
      previousTime(std::chrono::high_resolution_clock::now()),
      threadMessenger(),
      modelCenter(),
      modelPosition() {
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

  windowSurface = instance.CreateSurface(Win32SurfaceCreateInfoBuilder()
                                             .SetHinstance(windowInfo.hinstance)
                                             .SetHwnd(windowInfo.hwnd));

  std::vector<PhysicalDevice> physicalDevices = instance.GetPhysicalDevices();

  if (physicalDevices.empty()) {
    throw std::runtime_error("No Vulkan-enabled GPUs found on the machine.");
  }

  Log(std::format("Found {} GPU(s):", physicalDevices.size()));
  for (int index = 0; index < physicalDevices.size(); ++index) {
    Log(std::format("[{}] {}", index,
                    physicalDevices[index].GetProperties().deviceName));
  }
  Log("Selecting GPU 0 as render target.");

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

  if (std::filesystem::exists(PipelineCacheFilename)) {
    pipelineCache =
        virtualDevice.LoadPipelineCache(ReadFile(PipelineCacheFilename));
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

  const Model model = ModelFromObjFile("resources/InterstellarRunner.obj");

  std::unordered_map<TexturedVertex, u16> uniqueVertices;
  std::vector<TexturedVertex> vertices;
  std::vector<u16> indices;

  glm::vec3 corner1 = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 corner2 = glm::vec3(0.0f, 0.0f, 0.0f);

  for (const auto& face : model.faces) {
    for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex) {
      ModelFaceVertex modelFaceVertex = face.faceVertices[vertexIndex];

      TexturedVertex vertex;
      vertex.position.x = model.vertices[modelFaceVertex.vertexIndex].x;
      vertex.position.y = model.vertices[modelFaceVertex.vertexIndex].y;
      vertex.position.z = model.vertices[modelFaceVertex.vertexIndex].z;
      vertex.textureCoordinate.x =
          model.textureVertices[modelFaceVertex.textureVertexIndex].u;
      vertex.textureCoordinate.y =
          model.textureVertices[modelFaceVertex.textureVertexIndex].v;

      if (uniqueVertices.contains(vertex)) {
        indices.push_back(uniqueVertices[vertex]);
      } else {
        const u16 index = static_cast<u16>(uniqueVertices.size());

        uniqueVertices.emplace(vertex, index);

        vertices.push_back(vertex);
        indices.push_back(index);

        if (corner1.x < vertex.position.x) {
          corner1.x = vertex.position.x;
        }
        if (corner1.y < vertex.position.y) {
          corner1.y = vertex.position.y;
        }
        if (corner1.z < vertex.position.z) {
          corner1.z = vertex.position.z;
        }
        if (corner2.x > vertex.position.x) {
          corner2.x = vertex.position.x;
        }
        if (corner2.y > vertex.position.y) {
          corner2.y = vertex.position.y;
        }
        if (corner2.z > vertex.position.z) {
          corner2.z = vertex.position.z;
        }
      }
    }
  }

  modelCenter = (corner1 + corner2) / 2.0f;
  modelSize = corner1 - corner2;

  indexCount = indices.size();

  vertexMemoryBuffer = TransferDataToGpuLocalMemory(
      shortExecutionCommandBuffer, vertices.data(),
      sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
  indexMemoryBuffer = TransferDataToGpuLocalMemory(
      shortExecutionCommandBuffer, indices.data(),
      sizeof(indices[0]) * indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

  const Bitmap bitmap = ReadBitmap("resources/InterstellarRunner.bmp");

  const float pictureWidth = static_cast<float>(bitmap.width);
  const float pictureHeight = static_cast<float>(bitmap.height);

  Buffer stagingBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(bitmap.size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  DeviceMemorySubAllocation stagingBufferMemory = deviceAllocator.BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(bitmap.data.data(), stagingBuffer.Size());

  Image textureImage =
      virtualDevice.CreateImage(ImageCreateInfoBuilder(IMAGE_2D)
                                    .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
                                    .SetExtent(Extent3DBuilder()
                                                   .SetWidth(bitmap.width)
                                                   .SetHeight(bitmap.height)
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
                              .SetWidth(bitmap.width)
                              .SetHeight(bitmap.height)));
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

  textureView = textureImage.CreateView(
      ImageViewCreateInfoBuilder()
          .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
          .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  texture = {.image = std::move(textureImage),
             .memory = std::move(textureImageMemory)};
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
  const std::array<VkVertexInputAttributeDescription, 2>
      vertexAttributeDescriptions{
          VertexInputAttributeDescriptionBuilder()
              .SetBinding(0)
              .SetLocation(0)
              .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
              .SetOffset(offsetof(TexturedVertex, position)),
          VertexInputAttributeDescriptionBuilder()
              .SetBinding(0)
              .SetLocation(1)
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
              .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT);
  perFrameDescriptorSetLayout = virtualDevice.CreateDescriptorSetLayout(
      DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
          &perFrameDescriptorSetLayoutBinding));
  constexpr const VkDescriptorSetLayoutBinding textureSamplerLayoutBinding =
      DescriptorSetLayoutBindingBuilder()
          .SetBinding(0)
          .SetDescriptorCount(1)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT);
  textureSamplerDescriptorSetLayout = virtualDevice.CreateDescriptorSetLayout(
      DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
          &textureSamplerLayoutBinding));
  const std::vector<const DescriptorSetLayout*> descriptorSetLayouts = {
      &perSceneDescriptorSetLayout, &perFrameDescriptorSetLayout,
      &textureSamplerDescriptorSetLayout};
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
      windowInfo.rect,
      ImGuiInstance(windowInfo.window, instance, targetPhysicalDevice,
                    virtualDevice, queue, renderPass,
                    shortExecutionCommandBuffer, fence, samples));

  const u32 swapchainImages = swapchain.GetImageCount();
  constexpr const std::array<VkDescriptorPoolSize, 3> descriptorPoolSizes{
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetDescriptorCount(1),
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC)
          .SetDescriptorCount(1),
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetDescriptorCount(1),
  };
  descriptorPool = virtualDevice.CreateDescriptorPool(
      DescriptorPoolCreateInfoBuilder()
          .SetPoolSizeCount(descriptorPoolSizes.size())
          .SetPPoolSizes(descriptorPoolSizes.data())
          .SetMaxSets(3));
  sceneDescriptorSet =
      descriptorPool.AllocateDescriptorSet(perSceneDescriptorSetLayout);
  textureSamplerDescriptorSet =
      descriptorPool.AllocateDescriptorSet(textureSamplerDescriptorSetLayout);

  viewTransformBuffer = DynamicUniformBuffer<PerFrameData>(
      swapchainImages, physicalDeviceProperties.limits, descriptorPool,
      perFrameDescriptorSetLayout, virtualDevice, deviceAllocator);

  projectionTransformBuffer = TransferDataToGpuLocalMemory(
      shortExecutionCommandBuffer, &projectionTransform,
      sizeof(projectionTransform), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

  DescriptorSet::WriteDescriptorSet projectionBufferWrite;
  sceneDescriptorSet.CreateBufferWrite(
      projectionTransformBuffer.buffer, VK_WHOLE_SIZE,
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, projectionBufferWrite);

  DescriptorSet::WriteDescriptorSet viewBufferWrite;
  viewTransformBuffer.CreateWriteDescriptorSet(viewBufferWrite);

  DescriptorSet::WriteDescriptorSet textureSamplerWrite;
  textureSamplerDescriptorSet.CreateImageSamplerWrite(
      textureView, textureSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      textureSamplerWrite);

  std::array<VkWriteDescriptorSet, 3> descriptorSetWrites{
      projectionBufferWrite, viewBufferWrite, textureSamplerWrite};
  virtualDevice.UpdateDescriptorSets(descriptorSetWrites.size(),
                                     descriptorSetWrites.data());

  for (u32 renderIndex = 0; renderIndex < swapchainImages; ++renderIndex) {
    CommandBuffer renderPassCommandBuffer =
        renderCommandPool.AllocatePrimaryCommandBuffer();

    swapchainRenderData.emplace_back(std::move<SwapchainRenderPass>(
        {.commandBuffer = std::move(renderPassCommandBuffer),
         .renderCompleteSemaphore = virtualDevice.CreateSemaphore(),
         .submitCompleteFence =
             virtualDevice.CreateFence(VK_FENCE_CREATE_SIGNALED_BIT)}));
  }
}

App::~App() {
  SDL_Quit();
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

  Log(pCallbackData->pMessage);
  return VK_FALSE;
}

App::BufferWithMemory App::TransferDataToGpuLocalMemory(
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
  WriteFile(PipelineCacheFilename, pipelineCache.GetPipelineCacheData());
  return 0;
}

void App::MainThread() {
  while (true) {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
      uiRenderer->ProcessEvent(event);

      switch (event.type) {
        case SDL_QUIT:
          threadMessenger.PostMessage(EventNotification::Unpaused);
          threadMessenger.PostMessage(EventNotification::Exited);
          return;

        case SDL_WINDOWEVENT:
          switch (event.window.event) {
            case SDL_WINDOWEVENT_MINIMIZED:
              threadMessenger.PostMessage(EventNotification::Paused);
              break;

            case SDL_WINDOWEVENT_RESTORED:
              threadMessenger.PostMessage(EventNotification::Unpaused);
              break;

            case SDL_WINDOWEVENT_SIZE_CHANGED:
              windowInfo.rect.width = event.window.data1;
              windowInfo.rect.height = event.window.data2;
              threadMessenger.PostMessage(EventNotification::Resized);
              break;
          }
          break;

        case SDL_KEYDOWN:
          if (event.key.repeat == 0) {
            keyboard.Keydown(event.key.keysym.sym);
          }
          break;

        case SDL_KEYUP:
          keyboard.Keyup(event.key.keysym.sym);
          break;

        case SDL_MOUSEMOTION:
          mouse.Movement(event.motion);
          break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          mouse.Button(event.button);
          break;
      }
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
            Log("Recreating swapchain.");
            InitializeSwapchain();
            Log("Swapchain recreated.");
            break;
        }
      }
    }
  } catch (const std::exception& exception) {
    Log(exception.what());
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
  uiRenderer->ShowKeyboardLayout(keyboard);

  constexpr float movementSpeed = 10.0f;

  glm::vec3 movement(0.0f);
  if (keyboard.IsKeyDown(SDLK_a)) {
    movement.x = 1.0f;
  }
  if (keyboard.IsKeyDown(SDLK_d)) {
    movement.x = -1.0f;
  }
  if (keyboard.IsKeyDown(SDLK_w)) {
    movement.y = 1.0f;
  }
  if (keyboard.IsKeyDown(SDLK_s)) {
    movement.y = -1.0f;
  }
  if (keyboard.IsKeyDown(SDLK_q)) {
    movement.z = -1.0f;
  }
  if (keyboard.IsKeyDown(SDLK_e)) {
    movement.z = 1.0f;
  }

  if ((std::abs(movement.x) + std::abs(movement.y) + std::abs(movement.z)) >
      0.0f) {
    const glm::vec3 normalizedMovement =
        glm::normalize(movement) * movementSpeed * deltaTime;
    modelPosition += normalizedMovement;
  }

  modelTransform.model =
      glm::translate(glm::mat4(1.0f), modelPosition - modelCenter);

  glm::mat4 cameraTransform(1.0f);
  cameraTransform = glm::translate(cameraTransform, modelPosition);

  const bool reverseView = keyboard.IsKeyDown(SDLK_c);

  glm::vec2 mouseDelta = mouse.GetPositionDeltaFromRightClick();

  if (reverseView) {
    mouseDelta.y = -mouseDelta.y;
  }

  cameraTransform =
      glm::rotate(cameraTransform,
                  glm::pi<float>() * (-mouseDelta.x / windowInfo.rect.width),
                  glm::vec3(0.0f, 1.0f, 0.0f));
  cameraTransform =
      glm::rotate(cameraTransform,
                  glm::pi<float>() * (mouseDelta.y / windowInfo.rect.height),
                  glm::vec3(1.0f, 0.0f, 0.0f));

  if (reverseView) {
    cameraTransform = glm::rotate(cameraTransform, glm::pi<float>(),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
  }

  const glm::vec3 cameraLookAt = modelPosition;
  const glm::vec3 cameraPosition =
      cameraTransform * glm::vec4(0.0f, 0.0f, -modelSize.z, 1.0f);

  viewTransformBuffer.Value().view =
      glm::lookAt(cameraPosition, cameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f));

  uiRenderer->ShowObjectsInScene(
      ObjectsInSceneInfo{.cameraRotation = glm::vec2(0.0f, 0.0f),
                         .cameraLookAt = cameraLookAt,
                         .cameraPosition = cameraPosition,
                         .modelPosition = &modelPosition});

  keyboard.ClearPressedKeys();
  uiRenderer->EndFrame();
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
        textureSamplerDescriptorSet);
    swapchainRender.commandBuffer.CmdBindVertexBuffers(
        vertexMemoryBuffer.buffer, 0);
    swapchainRender.commandBuffer.CmdBindIndexBuffer(indexMemoryBuffer.buffer,
                                                     VK_INDEX_TYPE_UINT16);
    swapchainRender.commandBuffer.CmdPushConstants(
        pipeline.GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0,
        sizeof(modelTransform), &modelTransform);
    swapchainRender.commandBuffer.CmdDrawIndexed(
        indexCount, /* instanceCount= */ 1);  // TODO: More instances
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
      Log("Selecting sRGB 32-bit BGRA swapchain format.");
      return format;
    }
  }

  Log("Selecting first available swapchain format (could not find sRGB).");
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
  Log("Selecting vertically synced swapchain present mode.");
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
