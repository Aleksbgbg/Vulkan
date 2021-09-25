#include "App.h"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <iostream>
#include <format>
#include <thread>
#include <memory>
#include <cstring>
#include <chrono>
#include <cmath>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <vulkan/structures/DeviceQueueCreateInfo.h>
#include <vulkan/Buffer.h>
#include <vulkan/structures/default.h>
#include <vulkan/structures/DescriptorSetLayoutBinding.h>
#include <vulkan/structures/SubpassDescription.h>
#include <vulkan/structures/AttachmentReference.h>
#include <vulkan/structures/ClearValue.h>
#include <vulkan/DescriptorPool.h>

#include "vulkan/util.h"
#include "build_definition.h"
#include "VertexBuilder.h"

void Log(const std::string_view text) {
  std::cout << text << std::endl;
}

App::WindowInfo App::InitSdl() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  Rect windowRect = {
      .position = glm::ivec2(50, 50),
      .size = glm::ivec2(1366, 760)
  };

  SDL_Window* window =
      SDL_CreateWindow(
          "Vulkan",
          windowRect.position.x,
          windowRect.position.y,
          windowRect.size.x,
          windowRect.size.y,
          SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  SDL_SysWMinfo sysWmInfo;
  SDL_VERSION(&sysWmInfo.version)
  SDL_GetWindowWMInfo(window, &sysWmInfo);

  return WindowInfo{
      .window = window,
      .hinstance = sysWmInfo.info.win.hinstance,
      .hwnd = sysWmInfo.info.win.window,
      .rect = windowRect
  };
}

struct Bitmap {
  u32 width;
  u32 height;
  u32 bytesPerPixel;

  u32 size;

  std::vector<u8> data;
};

Bitmap ReadBitmap(const char* const path) {
  const std::vector<u8> fileData = ReadFile(path);

#pragma pack(push, r1, 1)
  struct BitmapHeader {
    struct {
      struct {
        u8 char0;
        u8 char1;
      } fileType;

      u32 fileSize;

      i16 reserved0;
      i16 reserved1;

      u32 pixelDataOffset;
    } fileHeader;

    struct {
      u32 headerSize;

      i32 imageWidth;
      i32 imageHeight;

      u16 planes;

      u16 bitsPerPixel;

      u32 compression;
      u32 imageSize;

      i32 xPixelsPerMeter;
      i32 yPixelsPerMeter;

      u32 totalColors;
      u32 importantColors;
    } infoHeader;
  };
#pragma pack(pop, r1)

  const BitmapHeader bitmapHeader = *reinterpret_cast<const BitmapHeader*>(fileData.data());

  Bitmap bitmap;
  bitmap.width = bitmapHeader.infoHeader.imageWidth;
  bitmap.height = bitmapHeader.infoHeader.imageHeight;
  bitmap.bytesPerPixel = bitmapHeader.infoHeader.bitsPerPixel / 8;
  bitmap.size = bitmap.width * bitmap.height * bitmap.bytesPerPixel;
  bitmap.data = std::vector<u8>(bitmap.size);

  if (bitmap.height < 0) {
    std::memcpy(bitmap.data.data(), fileData.data() + bitmapHeader.fileHeader.pixelDataOffset, bitmap.size);
    bitmap.height = -bitmap.height;
  } else {
    const u32 bitmapPixels = bitmap.width * bitmap.height;
    const u32* const pixelData = reinterpret_cast<const u32* const>(fileData.data() + bitmapHeader.fileHeader.pixelDataOffset);
    u8* targetData = bitmap.data.data();

    for (u32 pixel = 0; pixel < bitmapPixels; ++pixel) {
      const u32 value = pixelData[pixel];

      const u8 red = value & 255;
      const u8 green = (value & (255 << 8)) >> 8;
      const u8 blue = (value & (255 << 16)) >> 16;
      const u8 alpha = (value & (255 << 24)) >> 24;

      u8* targetColors = targetData + (pixel * bitmap.bytesPerPixel);

      targetColors[0] = blue;
      targetColors[1] = green;
      targetColors[2] = red;
      targetColors[3] = alpha;
    }

//    const u32 rowSize = bitmap.width * bitmap.bytesPerPixel;
//    for (u32 row = 0; row < bitmap.height; ++row) {
//      const u32 dataOffset = row * rowSize;
//      std::memcpy(
//          bitmap.data.data() + dataOffset,
//          fileData.data() + bitmapHeader.fileHeader.fileSize - dataOffset - rowSize,
//          rowSize);
//    }
  }

  return bitmap;
}

std::vector<u16> GenerateIndices(const std::vector<Vertex>& vertices) {
//  std::vector<u16> indices = {
//      0, 1, 2, 1, 2, 3, // bottom face
//      0, 4, 5, 0, 1, 5, // back face
//      1, 3, 7, 1, 5, 7, // left face
//      0, 2, 6, 0, 4, 6, // right face
//      2, 6, 7, 2, 3, 7, // front face
//      4, 5, 6, 5, 6, 7, // top face
//  };
//  return indices;

  // return VertexBuilder(vertices).GenerateIndices();

  std::vector<u16> indices;

  for (u32 plane = 0; plane < 3; ++plane) {
    for (i32 value = -1; value < 2; value += 2) {
      const float planeValue = static_cast<float>(value);

      std::vector<u16> planeVertices;

      for (u16 vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex) {
        if (vertices[vertexIndex].pos[plane] == planeValue) {
          planeVertices.push_back(vertexIndex);
        }
      }

      u32 otherAxis1;
      u32 otherAxis2;

      if (value == 1) {
        otherAxis1 = (plane + 1) % 3;
        otherAxis2 = (plane + 2) % 3;
      } else {
        otherAxis1 = (plane + 2) % 3;
        otherAxis2 = (plane + 1) % 3;
      }

      const u32 axesToModify[] = { otherAxis1, otherAxis2, otherAxis1, otherAxis2 };

      u32 added = 1;
      indices.push_back(planeVertices[0]);

      glm::vec3 vertex = vertices[planeVertices[0]].pos;

      for (u32 index = 0; index < 4; ++index) {
        const u32 axisToModify = axesToModify[index];

        if (vertex[axisToModify] == 1.0f) {
          vertex[axisToModify] = -1.0f;
        } else {
          vertex[axisToModify] = 1.0f;
        }

        for (u16 vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex) {
          if (vertices[vertexIndex].pos == vertex) {
            ++added;
            indices.push_back(vertexIndex);

            if (added == 3) {
              indices.push_back(vertexIndex);
            }

            break;
          }
        }
      }
    }
  }

  return indices;
}

App::App()
    :
    windowInfo(InitSdl()),
    renderTransform(),
    previousTime(std::chrono::high_resolution_clock::now()),
    cubeTransform(glm::mat4(1.0f)),
    cameraRotation(glm::vec2(0.0f)),
    threadMessenger(), acquiredImage({ .exists = false, .index = 0 }) {
  const std::vector<VkExtensionProperties> availableExtensions =
      LoadArray(VulkanInstance::LoadInstanceExtensionProperties);
  const std::vector<const char*> requiredExtensions = {
      VK_KHR_SURFACE_EXTENSION_NAME,
      VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#ifdef VALIDATION
      VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
  };

  if (!RequiredValuesAreAvailable(availableExtensions, requiredExtensions, [](const VkExtensionProperties& property) {
    return property.extensionName;
  })) {
    throw std::runtime_error("Required extensions not available.");
  }

  const std::vector<VkLayerProperties> availableLayers = LoadArray(VulkanInstance::LoadInstanceLayerProperties);
  const std::vector<const char*> requiredLayers = {
#ifdef VALIDATION
      "VK_LAYER_KHRONOS_validation",
#endif
  };

  if (!RequiredValuesAreAvailable(availableLayers, requiredLayers, [](const VkLayerProperties& property) {
    return property.layerName;
  })) {
    throw std::runtime_error("Required layers not available.");
  }

  instance =
      VulkanInstance(
          InstanceCreateInfoBuilder()
              .SetApplicationInfo(ApplicationInfoBuilder().SetPApplicationName("Application").SetApplicationVersion(1))
              .SetEnabledExtensionCount(requiredExtensions.size())
              .SetPpEnabledExtensionNames(requiredExtensions.data())
              .SetEnabledLayerCount(requiredLayers.size())
              .SetPpEnabledLayerNames(requiredLayers.data()));

#ifdef VALIDATION
  debugMessenger =
      instance.CreateDebugUtilsMessenger(
          DebugUtilsMessengerCreateInfoExtBuilder()
              .SetMessageSeverity(
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
                  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
              .SetMessageType(
                  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                  | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
                  | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
              .SetPfnUserCallback(DebugCallback));
#endif

  windowSurface =
      instance.CreateSurface(
          Win32SurfaceCreateInfoBuilder()
              .SetHinstance(windowInfo.hinstance)
              .SetHwnd(windowInfo.hwnd));

  std::vector<PhysicalDevice> physicalDevices = instance.GetPhysicalDevices();

  if (physicalDevices.empty()) {
    throw std::runtime_error("No Vulkan-enabled GPUs found on the machine.");
  }

  Log(std::format("Found {} GPU(s):", physicalDevices.size()));
  for (int index = 0; index < physicalDevices.size(); ++index) {
    Log(std::format("[{}] {}", index, physicalDevices[index].GetProperties().deviceName));
  }
  Log("Selecting GPU 0 as render target.");

  targetPhysicalDevice = physicalDevices[0];
  VkPhysicalDeviceProperties physicalDeviceProperties = targetPhysicalDevice.GetProperties();

  VkSurfaceCapabilitiesKHR surfaceCapabilities = windowSurface.GetCapabilities(targetPhysicalDevice);
  minSwapchainImages = std::min(surfaceCapabilities.minImageCount + 1, surfaceCapabilities.maxImageCount);
  framesInFlight = minSwapchainImages;

  std::optional<u32> queueFamilyIndex =
      targetPhysicalDevice.FindAppropriateQueueFamily(
          VK_QUEUE_GRAPHICS_BIT, [this](const u32 queueFamilyIndex) {
            return windowSurface.IsSupportedByPhysicalDevice(targetPhysicalDevice, queueFamilyIndex);
          });

  if (!queueFamilyIndex.has_value()) {
    throw std::runtime_error("Could not find device queue family with required features.");
  }

  const std::vector<const char*> deviceExtensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  virtualDevice =
      targetPhysicalDevice.CreateVirtualDevice(
          DeviceCreateInfoBuilder()
              .SetQueueCreateInfoCount(1)
              .SetPQueueCreateInfos(
                  DeviceQueueCreateInfoBuilder()
                      .SetQueueCount(1)
                      .SetQueueFamilyIndex(queueFamilyIndex.value())
                      .SetPQueuePriorities(std::vector<float>{ 1.0f }.data())
                      .Build())
              .SetPEnabledFeatures(
                  PhysicalDeviceFeaturesBuilder().SetGeometryShader(VK_TRUE).SetSamplerAnisotropy(VK_TRUE))
              .SetEnabledExtensionCount(deviceExtensions.size())
              .SetPpEnabledExtensionNames(deviceExtensions.data()));
  queue = virtualDevice.GetQueue(queueFamilyIndex.value(), 0);

  const std::vector<Vertex> vertices = {
      /* 0 */ {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      /* 1 */ {{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
      /* 2 */ {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
      /* 3 */ {{1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
      /* 4 */ {{-1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
      /* 5 */ {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
      /* 6 */ {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      /* 7 */ {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
  };

  const std::vector<u16> indices = GenerateIndices(vertices);
  indexCount = indices.size();

  CommandPool memoryTransferCommandPool =
      queue.CreateCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
  CommandBuffer memoryTransferCommandBuffer = memoryTransferCommandPool.AllocatePrimaryCommandBuffer();

  vertexMemoryBuffer =
      TransferDataToGpuLocalMemory(
          memoryTransferCommandBuffer,
          vertices.data(),
          sizeof(vertices[0]) * vertices.size(),
          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
  indexMemoryBuffer =
      TransferDataToGpuLocalMemory(
          memoryTransferCommandBuffer,
          indices.data(),
          sizeof(indices[0]) * indices.size(),
          VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

  Bitmap bitmap = ReadBitmap("Alice.bmp");

  Buffer stagingBuffer =
      virtualDevice.CreateBuffer(
          BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
              .SetSize(bitmap.size)
              .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  DeviceMemory stagingBufferMemory =
      stagingBuffer.AllocateAndBindMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(bitmap.data.data(), stagingBuffer.Size());

  Image textureImage =
      virtualDevice.CreateImage(
          ImageCreateInfoBuilder(IMAGE_2D)
              .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
              .SetExtent(Extent3DBuilder().SetWidth(bitmap.width).SetHeight(bitmap.height).SetDepth(1))
              .SetUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT));
  DeviceMemory textureImageMemory = textureImage.AllocateAndBindMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  memoryTransferCommandBuffer.BeginOneTimeSubmit();
  memoryTransferCommandBuffer.CmdImageMemoryBarrier(
      textureImage,
      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(0)
          .SetDstAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetNewLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  memoryTransferCommandBuffer.CmdCopyBufferToImage(
      stagingBuffer,
      textureImage,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      BufferImageCopyBuilder()
          .SetImageSubresource(SUBRESOURCE_LAYERS_COLOR_SINGLE_LAYER)
          .SetImageExtent(Extent3DBuilder(EXTENT3D_SINGLE_DEPTH).SetWidth(bitmap.width).SetHeight(bitmap.height)));
  memoryTransferCommandBuffer.CmdImageMemoryBarrier(
      textureImage,
      VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetDstAccessMask(VK_ACCESS_SHADER_READ_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetNewLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  memoryTransferCommandBuffer.End().Submit().Wait();

  textureView =
      textureImage.CreateView(
          ImageViewCreateInfoBuilder()
              .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
              .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
              .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  texture = {
      .image = std::move(textureImage),
      .memory = std::move(textureImageMemory)
  };
  textureSampler =
      virtualDevice.CreateSampler(
          SamplerCreateInfoBuilder()
              .SetMagFilter(VK_FILTER_LINEAR)
              .SetMinFilter(VK_FILTER_LINEAR)
              .SetAddressModeU(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAddressModeV(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAddressModeW(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAnisotropyEnable(VK_TRUE)
              .SetMaxAnisotropy(physicalDeviceProperties.limits.maxSamplerAnisotropy));

  renderCommandPool = queue.CreateCommandPool();

  shaders.emplace_back(virtualDevice.LoadShader(VK_SHADER_STAGE_VERTEX_BIT, "shaders/shader.vert.spv"));
  shaders.emplace_back(virtualDevice.LoadShader(VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/shader.frag.spv"));

  InitializeSwapchain();

  for (u32 inFlightImage = 0; inFlightImage < framesInFlight; ++inFlightImage) {
    imagesInFlightSynchronisation.emplace_back(std::move<InFlightImage>({
      .acquireImageSemaphore = virtualDevice.MakeSemaphore()
    }));
  }
}

void App::InitializeSwapchain() {
  VkSurfaceCapabilitiesKHR surfaceCapabilities = windowSurface.GetCapabilities(targetPhysicalDevice);
  VkSurfaceFormatKHR surfaceFormat = SelectSwapSurfaceFormat(windowSurface.GetFormats(targetPhysicalDevice));
  swapchain =
      virtualDevice.CreateSwapchain(
          windowSurface,
          SwapchainCreateInfoBuilder()
              .SetMinImageCount(minSwapchainImages)
              .SetImageFormat(surfaceFormat.format)
              .SetImageColorSpace(surfaceFormat.colorSpace)
              .SetImageExtent(SelectSwapExtent(surfaceCapabilities))
              .SetImageArrayLayers(1)
              .SetImageUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
              .SetImageSharingMode(VK_SHARING_MODE_EXCLUSIVE)
              .SetPreTransform(surfaceCapabilities.currentTransform)
              .SetCompositeAlpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
              .SetPresentMode(SelectSwapPresentMode(windowSurface.GetPresentModes(targetPhysicalDevice)))
              .SetClipped(VK_TRUE));

  VkFormat depthStencilFormat =
      SelectDepthStencilFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT });

  const VkExtent2D swapchainExtent = swapchain.GetImageExtent();

  renderTransform.proj =
      glm::perspective(
          glm::radians(45.0f),
          static_cast<float>(swapchainExtent.width) / static_cast<float>(swapchainExtent.height),
          0.1f,
          1000.0f);
  renderTransform.proj[1][1] *= -1;

  Image depthStencilImage =
      virtualDevice.CreateImage(
          ImageCreateInfoBuilder(IMAGE_2D)
              .SetFormat(depthStencilFormat)
              .SetExtent(Extent3DBuilder(swapchainExtent).SetDepth(1))
              .SetUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
  DeviceMemory depthStencilMemory = depthStencilImage.AllocateAndBindMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  depthStencilView =
      depthStencilImage.CreateView(
          ImageViewCreateInfoBuilder()
              .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
              .SetSubresourceRange(
                  ImageSubresourceRangeBuilder()
                      .SetAspectMask(VK_IMAGE_ASPECT_DEPTH_BIT)
                      .SetLevelCount(1)
                      .SetLayerCount(1)));

  depthStencil = {
      .image = std::move(depthStencilImage),
      .memory = std::move(depthStencilMemory)
  };

  const std::array<VkAttachmentDescription, 2> attachmentDescriptions {
      AttachmentDescriptionBuilder()
          .SetFormat(swapchain.GetImageFormat())
          .SetSamples(VK_SAMPLE_COUNT_1_BIT)
          .SetLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
          .SetStoreOp(VK_ATTACHMENT_STORE_OP_STORE)
          .SetStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
          .SetStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetFinalLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
          .BuildObject(),
      AttachmentDescriptionBuilder()
          .SetFormat(depthStencilFormat)
          .SetSamples(VK_SAMPLE_COUNT_1_BIT)
          .SetLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
          .SetStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
          .SetStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetFinalLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
          .BuildObject(),
  };
  const std::array<VkVertexInputBindingDescription, 1> vertexBindingDescriptions {
      VertexInputBindingDescriptionBuilder()
          .SetBinding(0)
          .SetStride(sizeof(Vertex))
          .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX)
          .BuildObject(),
  };
  const std::array<VkVertexInputAttributeDescription, 3> vertexAttributeDescriptions {
      VertexInputAttributeDescriptionBuilder()
          .SetBinding(0)
          .SetLocation(0)
          .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
          .SetOffset(offsetof(Vertex, pos))
          .BuildObject(),
      VertexInputAttributeDescriptionBuilder()
          .SetBinding(0)
          .SetLocation(1)
          .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
          .SetOffset(offsetof(Vertex, color))
          .BuildObject(),
      VertexInputAttributeDescriptionBuilder()
          .SetBinding(0)
          .SetLocation(2)
          .SetFormat(VK_FORMAT_R32G32_SFLOAT)
          .SetOffset(offsetof(Vertex, texCoord))
          .BuildObject(),
  };
  const std::array<VkDescriptorSetLayoutBinding, 2> descriptorSetLayouts {
      DescriptorSetLayoutBindingBuilder()
          .SetBinding(0)
          .SetDescriptorCount(1)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT)
          .BuildObject(),
      DescriptorSetLayoutBindingBuilder()
          .SetBinding(1)
          .SetDescriptorCount(1)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT)
          .BuildObject(),
  };
  descriptorSetLayout =
      virtualDevice.CreateDescriptorSetLayout(
          DescriptorSetLayoutCreateInfoBuilder()
              .SetBindingCount(descriptorSetLayouts.size())
              .SetPBindings(descriptorSetLayouts.data()));
  pipeline =
      virtualDevice.CreateGraphicsPipeline(
          shaders,
          virtualDevice.CreatePipelineLayout(descriptorSetLayout),
          virtualDevice.CreateRenderPass(
              RenderPassCreateInfoBuilder()
                  .SetAttachmentCount(attachmentDescriptions.size())
                  .SetPAttachments(attachmentDescriptions.data())
                  .SetSubpassCount(1)
                  .SetPSubpasses(
                      SubpassDescriptionBuilder()
                          .SetPipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
                          .SetColorAttachmentCount(1)
                          .SetPColorAttachments(
                              AttachmentReferenceBuilder()
                                  .SetAttachment(0)
                                  .SetLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL))
                          .SetPDepthStencilAttachment(
                              AttachmentReferenceBuilder()
                                  .SetAttachment(1)
                                  .SetLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)))
                  .SetDependencyCount(1)
                  .SetPDependencies(
                      SubpassDependencyBuilder()
                          .SetSrcSubpass(VK_SUBPASS_EXTERNAL)
                          .SetDstSubpass(0)
                          .SetSrcStageMask(
                              VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)
                          .SetDstStageMask(
                              VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)
                          .SetSrcAccessMask(0)
                          .SetDstAccessMask(
                              VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT))),
          GraphicsPipelineCreateInfoBuilder()
              .SetPDepthStencilState(
                  PipelineDepthStencilStateCreateInfoBuilder()
                      .SetDepthTestEnable(VK_TRUE)
                      .SetDepthWriteEnable(VK_TRUE)
                      .SetDepthCompareOp(VK_COMPARE_OP_LESS)
                      .SetDepthBoundsTestEnable(VK_FALSE))
              .SetPVertexInputState(
                  PipelineVertexInputStateCreateInfoBuilder()
                      .SetVertexBindingDescriptionCount(vertexBindingDescriptions.size())
                      .SetPVertexBindingDescriptions(vertexBindingDescriptions.data())
                      .SetVertexAttributeDescriptionCount(vertexAttributeDescriptions.size())
                      .SetPVertexAttributeDescriptions(vertexAttributeDescriptions.data()))
              .SetPInputAssemblyState(
                  PipelineInputAssemblyStateCreateInfoBuilder().SetTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST))
              .SetPViewportState(
                  PipelineViewportStateCreateInfoBuilder()
                      .SetViewportCount(1)
                      .SetPViewports(
                          ViewportBuilder(VIEWPORT_BASE)
                              .SetWidth(static_cast<float>(swapchainExtent.width))
                              .SetHeight(static_cast<float>(swapchainExtent.height)))
                      .SetScissorCount(1)
                      .SetPScissors(Rect2DBuilder().SetOffset(OFFSET2D_ZERO).SetExtent(swapchainExtent)))
              .SetPRasterizationState(
                  PipelineRasterizationStateCreateInfoBuilder()
                      .SetCullMode(VK_CULL_MODE_BACK_BIT)
                      .SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE)
                      .SetPolygonMode(VK_POLYGON_MODE_FILL)
                      .SetLineWidth(1.0f))
              .SetPMultisampleState(
                  PipelineMultisampleStateCreateInfoBuilder()
                      .SetRasterizationSamples(VK_SAMPLE_COUNT_1_BIT)
                      .SetMinSampleShading(1.0f))
              .SetPColorBlendState(
                  PipelineColorBlendStateCreateInfoBuilder()
                      .SetAttachmentCount(1)
                      .SetPAttachments(
                          PipelineColorBlendAttachmentStateBuilder()
                              .SetColorWriteMask(
                                  VK_COLOR_COMPONENT_R_BIT
                                  | VK_COLOR_COMPONENT_G_BIT
                                  | VK_COLOR_COMPONENT_B_BIT
                                  | VK_COLOR_COMPONENT_A_BIT)
                              .SetSrcColorBlendFactor(VK_BLEND_FACTOR_ONE)
                              .SetDstColorBlendFactor(VK_BLEND_FACTOR_ZERO)
                              .SetColorBlendOp(VK_BLEND_OP_ADD)
                              .SetSrcAlphaBlendFactor(VK_BLEND_FACTOR_ONE)
                              .SetDstAlphaBlendFactor(VK_BLEND_FACTOR_ZERO)
                              .SetAlphaBlendOp(VK_BLEND_OP_ADD))));
  swapchainFramebuffers = swapchain.GetFramebuffers(pipeline.GetRenderPass(), depthStencilView);

  const u32 swapchainImages = swapchain.GetImageCount();
  const std::array<VkDescriptorPoolSize, 2> descriptorPoolSizes {
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetDescriptorCount(swapchainImages)
          .BuildObject(),
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetDescriptorCount(swapchainImages)
          .BuildObject(),
  };
  descriptorPool =
      virtualDevice.CreateDescriptorPool(
          DescriptorPoolCreateInfoBuilder()
              .SetPoolSizeCount(descriptorPoolSizes.size())
              .SetPPoolSizes(descriptorPoolSizes.data())
              .SetMaxSets(swapchainImages));
  std::vector<DescriptorSet> descriptorSets =
      descriptorPool.AllocateDescriptorSets(descriptorSetLayout, swapchainImages);

  const std::array<VkClearValue, 2> clearValues{
      ClearValueBuilder()
          .SetColor(ClearColorValueBuilder().SetFloat0(0.2f).SetFloat1(0.2f).SetFloat2(0.2f).SetFloat3(1.0f))
          .BuildObject(),
      ClearValueBuilder()
          .SetDepthStencil(ClearDepthStencilValueBuilder().SetDepth(1.0f))
          .BuildObject()
  };
  for (u32 renderIndex = 0; renderIndex < swapchainImages; ++renderIndex) {
    Buffer uniformBuffer =
        virtualDevice.CreateBuffer(
            BufferCreateInfoBuilder()
                .SetUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
                .SetSize(sizeof(UniformBufferObject))
                .SetSharingMode(VK_SHARING_MODE_EXCLUSIVE));
    DeviceMemory uniformBufferMemory =
        uniformBuffer.AllocateAndBindMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    DescriptorSet& descriptorSet = descriptorSets[renderIndex];
    DescriptorSet::WriteDescriptorSetBuild bufferWrite = descriptorSet.CreateBufferWrite(uniformBuffer);
    DescriptorSet::WriteDescriptorSetBuild textureSamplerWrite =
        descriptorSet
            .CreateImageSamplerWrite(textureView, textureSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    std::array<VkWriteDescriptorSet, 2> descriptorSetWrites{
        bufferWrite.builder.SetPBufferInfo(&bufferWrite.info.bufferInfo).BuildObject(),
        textureSamplerWrite
            .builder
            .SetPImageInfo(&textureSamplerWrite.info.imageInfo)
            .SetDstBinding(1)
            .BuildObject()
    };
    virtualDevice.UpdateDescriptorSets(descriptorSetWrites.size(), descriptorSetWrites.data());

    CommandBuffer renderPassCommandBuffer = renderCommandPool.AllocatePrimaryCommandBuffer();
    renderPassCommandBuffer.Begin();
    renderPassCommandBuffer.CmdBeginRenderPass(
        RenderPassBeginInfoBuilder()
            .SetRenderArea(Rect2DBuilder().SetExtent(swapchainExtent))
            .SetClearValueCount(clearValues.size())
            .SetPClearValues(clearValues.data()),
        VK_SUBPASS_CONTENTS_INLINE,
        pipeline.GetRenderPass(),
        swapchainFramebuffers[renderIndex]);
    renderPassCommandBuffer.CmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    renderPassCommandBuffer.CmdBindVertexBuffers(vertexMemoryBuffer.buffer);
    renderPassCommandBuffer.CmdBindIndexBuffer(indexMemoryBuffer.buffer, VK_INDEX_TYPE_UINT16);
    renderPassCommandBuffer.CmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetLayout(), descriptorSets[0]);
    renderPassCommandBuffer.CmdDrawIndexed(indexCount, /* instanceCount= */ 1); // TODO: More instances
    renderPassCommandBuffer.CmdEndRenderPass();
    renderPassCommandBuffer.End();

    swapchainRenderData.emplace_back(std::move<SwapchainRenderPass>({
        .renderData = {
            .buffer = std::move(uniformBuffer),
            .memory = std::move(uniformBufferMemory)
        },
        .commandBuffer = std::move(renderPassCommandBuffer),
        .renderCompleteSemaphore = virtualDevice.MakeSemaphore(),
        .submitCompleteFence = virtualDevice.MakeFence(VK_FENCE_CREATE_SIGNALED_BIT)
    }));
  }
}

App::~App() {
  SDL_Quit();
  virtualDevice.WaitIdle();
}

VkBool32
App::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
  if ((pCallbackData->messageIdNumber == 0xc036022f)
      || (pCallbackData->messageIdNumber == 0x618ab1e7)) {
    return VK_FALSE;
  }

  Log(pCallbackData->pMessage);
  return VK_FALSE;
}

App::BufferWithMemory App::TransferDataToGpuLocalMemory(
    CommandBuffer& commandBuffer, const void* data, const u32 size, const VkBufferUsageFlags usage) const {
  Buffer stagingBuffer =
      virtualDevice.CreateBuffer(
          BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
              .SetSize(size)
              .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  DeviceMemory stagingBufferMemory =
      stagingBuffer.AllocateAndBindMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(data, stagingBuffer.Size());

  Buffer finalBuffer =
      virtualDevice.CreateBuffer(
          BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
              .SetSize(size)
              .SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage));
  DeviceMemory finalBufferMemory = finalBuffer.AllocateAndBindMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  commandBuffer.BeginOneTimeSubmit();
  commandBuffer.CmdCopyBufferFull(stagingBuffer, finalBuffer);
  commandBuffer.End().Submit().Wait();

  return {
      .buffer = std::move(finalBuffer),
      .memory = std::move(finalBufferMemory)
  };
}

int App::Run() {
  std::thread renderThread(&App::RenderThread, this);
  MainThread();
  renderThread.join();
  return 0;
}

void App::MainThread() {
  while (true) {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
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
              windowInfo.rect.size = glm::ivec2(event.window.data1, event.window.data2);
              threadMessenger.PostMessage(EventNotification::Resized);
              break;
          }
          break;

        case SDL_KEYDOWN:
          keyboard.Keydown(event.key.keysym.sym);
          break;

        case SDL_KEYUP:
          keyboard.Keyup(event.key.keysym.sym);
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
            acquiredImage.exists = false;
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
  const float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(timeNow - previousTime).count();

  UpdateModel(deltaTime);
  Render();

  previousTime = timeNow;
}

void App::UpdateModel(const float deltaTime) {
  constexpr float movementSpeed = 50.0f;
  if (keyboard.IsKeyDown(SDLK_a)) {
    cubeTransform[3][0] -= deltaTime * movementSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_d)) {
    cubeTransform[3][0] += deltaTime * movementSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_w)) {
    cubeTransform[3][1] += deltaTime * movementSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_s)) {
    cubeTransform[3][1] -= deltaTime * movementSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_q)) {
    cubeTransform[3][2] -= deltaTime * movementSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_e)) {
    cubeTransform[3][2] += deltaTime * movementSpeed;
  }

  constexpr float cameraRotationSpeed = 75.0f;
  if (keyboard.IsKeyDown(SDLK_LEFT)) {
    cameraRotation.x -= deltaTime * cameraRotationSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_RIGHT)) {
    cameraRotation.x += deltaTime * cameraRotationSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_UP)) {
    cameraRotation.y += deltaTime * cameraRotationSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_DOWN)) {
    cameraRotation.y -= deltaTime * cameraRotationSpeed;
  }

  cubeRotation += deltaTime * 90.0f;

  constexpr glm::vec3 cameraStarePoint = glm::vec3(0.0f, -2.0f, -4.0f);
  const glm::mat4 cameraCenteredCube = glm::translate(cubeTransform, cameraStarePoint);
  const glm::mat4 rotatedCube = glm::rotate(cameraCenteredCube, glm::radians(cubeRotation), glm::vec3(0.0f, 1.0f, 0.0f));

  const glm::mat4 rotateWorldRelativeToCamera =
      glm::rotate(
          glm::rotate(
              glm::mat4(1.0f),
              glm::radians(cameraRotation.x),
              glm::vec3(0.0f, 1.0f, 0.0f)),
          glm::radians(cameraRotation.y),
          glm::vec3(1.0f, 0.0f, 0.0f));
  const glm::vec4 cameraRotatedCenter = rotateWorldRelativeToCamera * glm::vec4(cameraStarePoint, 1.0f);

  renderTransform.model = rotatedCube;
  renderTransform.view = glm::lookAt(glm::vec3(0.0f), glm::vec3(cameraRotatedCenter), glm::vec3(0.0f, 1.0f, 0.0f));
}

void App::Render() {
  InFlightImage& synchronisation = imagesInFlightSynchronisation[currentInFlightImage];

  u32 imageIndex;

  if (acquiredImage.exists) {
    acquiredImage.exists = false;
    imageIndex = acquiredImage.index;
  } else {
    const Swapchain::AcquireNextImageResult nextImageResult =
        swapchain.AcquireNextImage(SynchronisationPack().SetSignalSemaphore(&synchronisation.acquireImageSemaphore));
    imageIndex = nextImageResult.imageIndex;

    if ((nextImageResult.status == VK_SUBOPTIMAL_KHR) || (nextImageResult.status == VK_ERROR_OUT_OF_DATE_KHR)) {
      acquiredImage.exists = true;
      acquiredImage.index = imageIndex;
      Log("A");
      // This semaphore no longer has a way to be signalled
      // synchronisation.acquireImageSemaphore = virtualDevice.MakeSemaphore();
      return;
    }
  }

  SwapchainRenderPass& swapchainRender = swapchainRenderData[imageIndex];

  swapchainRender.renderData.memory.MapCopy(&renderTransform, sizeof(renderTransform));

  swapchainRender.submitCompleteFence.Wait().Reset();
  swapchainRender.commandBuffer.Submit(
      SynchronisationPack()
          .SetWaitSemaphore(&synchronisation.acquireImageSemaphore)
          .SetSignalSemaphore(&swapchainRender.renderCompleteSemaphore)
          .SetSignalFence(&swapchainRender.submitCompleteFence));
  const VkResult result =
      queue.Present(
          swapchain, imageIndex, SynchronisationPack().SetWaitSemaphore(&swapchainRender.renderCompleteSemaphore));

  if ((result == VK_SUBOPTIMAL_KHR) || (result == VK_ERROR_OUT_OF_DATE_KHR)) {
    // swapchainRender.submitCompleteFence.Wait();
    // synchronisation.acquireImageSemaphore = virtualDevice.MakeSemaphore();
    Log("B");
    return;
  }

  currentInFlightImage = (currentInFlightImage + 1) % framesInFlight;
}

VkSurfaceFormatKHR App::SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const {
  for (const auto& format : availableFormats) {
    if ((format.format == VK_FORMAT_B8G8R8A8_SRGB) &&
        (format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)) {
      // auto f = targetPhysicalDevice.GetFormatProperties(format.format);
      // VkImageFormatProperties formatProperties = targetPhysicalDevice.GetSurfaceImageFormatProperties(format);

      Log("Selecting sRGB 32-bit BGRA swapchain format.");
      return format;
    }
  }

  Log("Selecting first available swapchain format (could not find sRGB).");
  return availableFormats[0];
}

VkExtent2D App::SelectSwapExtent(const VkSurfaceCapabilitiesKHR surfaceCapabilities) {
  // TODO: Check the framebuffer size if the currentExtent is a special value UINT32_MAX
  // meaning the picture can be a different size than the window
  return surfaceCapabilities.currentExtent;
}

VkPresentModeKHR App::SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
  for (const auto& presentMode : availablePresentModes) {
    if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      Log("Selecting immediate render vertical sync swapchain present mode.");
      return presentMode;
    }
  }

  Log("Selecting vertically synced swapchain present mode.");
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkFormat App::SelectDepthStencilFormat(const std::vector<VkFormat>& potentialFormats) const {
  for (const VkFormat format : potentialFormats) {
    const VkFormatProperties formatProperties = targetPhysicalDevice.GetFormatProperties(format);

    if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
      return format;
    }
  }

  throw std::runtime_error("Could not find suitable depth stencil format.");
}
