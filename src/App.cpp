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

#include <vulkan/structures/DeviceQueueCreateInfo.h>
#include <vulkan/Buffer.h>
#include <vulkan/structures/default.h>
#include <vulkan/structures/DescriptorSetLayoutBinding.h>
#include <vulkan/structures/SubpassDescription.h>
#include <vulkan/structures/AttachmentReference.h>
#include <vulkan/structures/ClearValue.h>
#include <vulkan/DescriptorPool.h>
#include <vulkan/structures/PushConstantRange.h>

#include "vulkan/util.h"
#include "build_definition.h"
#include "VertexBuilder.h"
#include "GradientVertex.h"

struct UiVertex {
  glm::vec2 position;
  glm::vec2 textureCoordinate;
};

void Log(const std::string_view text) {
  std::cout << text << std::endl;
}

App::WindowInfo App::InitSdl() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  Rect windowRect = {
      .position = glm::ivec2(50, 50),
      .size = glm::ivec2(1920, 1080)
  };

  SDL_Window* window =
      SDL_CreateWindow(
          "Vulkan",
          windowRect.position.x,
          windowRect.position.y,
          windowRect.size.x,
          windowRect.size.y,
          SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
//  SDL_GetWindowSize(window, &windowRect.size.x, &windowRect.size.y);
//  SDL_Vulkan_GetDrawableSize(window, &windowRect.size.x, &windowRect.size.y);

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

std::vector<u16> GenerateIndices(const std::vector<GradientVertex>& vertices) {
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
        if (vertices[vertexIndex].position[plane] == planeValue) {
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

      glm::vec3 vertex = vertices[planeVertices[0]].position;

      for (u32 index = 0; index < 4; ++index) {
        const u32 axisToModify = axesToModify[index];

        if (vertex[axisToModify] == 1.0f) {
          vertex[axisToModify] = -1.0f;
        } else {
          vertex[axisToModify] = 1.0f;
        }

        for (u16 vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex) {
          if (vertices[vertexIndex].position == vertex) {
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
    gradientRenderTransform(),
    previousTime(std::chrono::high_resolution_clock::now()),
    cubePosition(),
    gradientCubePosition(),
    cameraRotation(glm::vec2(0.0f)),
    threadMessenger(),
    acquiredImage({ .exists = false, .index = 0 }) {
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
  physicalDeviceProperties = targetPhysicalDevice.GetProperties();

  VkSurfaceCapabilitiesKHR surfaceCapabilities = windowSurface.GetCapabilities(targetPhysicalDevice);
  minSwapchainImages = std::min(surfaceCapabilities.minImageCount + 1, surfaceCapabilities.maxImageCount);
  framesInFlight = minSwapchainImages;

  const std::optional<u32> queueFamilyIndex =
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

  deviceAllocator = virtualDevice.CreateMemoryAllocator();

  fence = virtualDevice.CreateFence();

  CommandPool memoryTransferCommandPool =
      queue.CreateCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
  CommandBuffer memoryTransferCommandBuffer = memoryTransferCommandPool.AllocatePrimaryCommandBuffer();

  const std::vector<TexturedVertex> vertices = {
      // Sides:
      /* 0  */ {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
      /* 1  */ {{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}},
      /* 2  */ {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f}},
      /* 3  */ {{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}},
      /* 4  */ {{-1.0f, 1.0f, -1.0f}, {1.0f, 0.0f}},
      /* 5  */ {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}},
      /* 6  */ {{-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
      /* 7  */ {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
      // Top
      /* 8  */ {{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f}},
      /* 9  */ {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f}},
      /* 10 */ {{-1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
      /* 11 */ {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
      // Bottom
      /* 12 */ {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
      /* 13 */ {{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}},
      /* 14 */ {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}},
      /* 15 */ {{1.0f, -1.0f, 1.0f}, {1.0f, 1.0f}},
  };
  const std::vector<u16> indices = {
      12, 13, 15, 15, 14, 12, // bottom face
      0, 4, 5, 5, 1, 0, // back face
      0, 2, 6, 6, 4, 0, // left face
      3, 1, 5, 5, 7, 3, // right face
      2, 3, 7, 7, 6, 2, // front face
      8, 10, 11, 11, 9, 8, // top face
  };
  indexCount = indices.size();

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

  const std::vector<GradientVertex> gradientVertices = {
      /* 0 */ {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
      /* 1 */ {{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
      /* 2 */ {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
      /* 3 */ {{1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
      /* 4 */ {{-1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
      /* 5 */ {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
      /* 6 */ {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
      /* 7 */ {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
  };
  const std::vector<u16> gradientIndices = GenerateIndices(gradientVertices);
  gradientIndexCount = gradientIndices.size();

  gradientVertexMemoryBuffer =
      TransferDataToGpuLocalMemory(
          memoryTransferCommandBuffer,
          gradientVertices.data(),
          sizeof(gradientVertices[0]) * gradientVertices.size(),
          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
  gradientIndexMemoryBuffer =
      TransferDataToGpuLocalMemory(
          memoryTransferCommandBuffer,
          gradientIndices.data(),
          sizeof(gradientIndices[0]) * gradientIndices.size(),
          VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

  Bitmap bitmap = ReadBitmap("Alice.bmp");

  const float pictureWidth = static_cast<float>(bitmap.width);
  const float pictureHeight = static_cast<float>(bitmap.height);

  Buffer stagingBuffer =
      virtualDevice.CreateBuffer(
          BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
              .SetSize(bitmap.size)
              .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  ReservedMemory stagingBufferMemory =
      deviceAllocator.BindMemory(stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.memoryBinding.GetMemory().MapCopy(bitmap.data.data(), stagingBufferMemory.offset, stagingBuffer.Size());

  Image textureImage =
      virtualDevice.CreateImage(
          ImageCreateInfoBuilder(IMAGE_2D)
              .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
              .SetExtent(Extent3DBuilder().SetWidth(bitmap.width).SetHeight(bitmap.height).SetDepth(1))
              .SetUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT));
  ReservedMemory textureImageMemory =
      deviceAllocator.BindMemory(textureImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

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
  memoryTransferCommandBuffer.End().Submit(fence).Wait().Reset();

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

  renderCommandPool = queue.CreateCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

  shaders.emplace_back(virtualDevice.LoadShader(VK_SHADER_STAGE_VERTEX_BIT, "shaders/textured_cube.vert.spv"));
  shaders.emplace_back(virtualDevice.LoadShader(VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/textured_cube.frag.spv"));
  gradientShaders.emplace_back(virtualDevice.LoadShader(VK_SHADER_STAGE_VERTEX_BIT, "shaders/gradient_cube.vert.spv"));
  gradientShaders.emplace_back(virtualDevice.LoadShader(VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/gradient_cube.frag.spv"));

  InitializeSwapchain(memoryTransferCommandBuffer);

  for (u32 inFlightImage = 0; inFlightImage < framesInFlight; ++inFlightImage) {
    imagesInFlightSynchronisation.emplace_back(std::move<InFlightImage>({
      .acquireImageSemaphore = virtualDevice.CreateSemaphore()
    }));
  }
}

void App::InitializeSwapchain(CommandBuffer& transientCommandBuffer) {
  VkSurfaceCapabilitiesKHR surfaceCapabilities = windowSurface.GetCapabilities(targetPhysicalDevice);
  VkSurfaceFormatKHR surfaceFormat = SelectSwapSurfaceFormat(windowSurface.GetFormats(targetPhysicalDevice));
  swapchain =
      virtualDevice.CreateSwapchain(
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
              .SetPresentMode(SelectSwapPresentMode(windowSurface.GetPresentModes(targetPhysicalDevice)))
              .SetClipped(VK_TRUE));

  VkFormat depthStencilFormat =
      SelectDepthStencilFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT });

  const VkExtent2D swapchainExtent = swapchain.GetImageExtent();

  projectionTransform.value =
      glm::perspective(
          glm::radians(45.0f),
          static_cast<float>(swapchainExtent.width) / static_cast<float>(swapchainExtent.height),
          0.1f,
          1000.0f);
  projectionTransform.value[1][1] *= -1;

  Image depthStencilImage =
      virtualDevice.CreateImage(
          ImageCreateInfoBuilder(IMAGE_2D)
              .SetFormat(depthStencilFormat)
              .SetExtent(Extent3DBuilder(swapchainExtent).SetDepth(1))
              .SetUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
  ReservedMemory depthStencilMemory =
      deviceAllocator.BindMemory(depthStencilImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
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
  const std::array<VkAttachmentReference, 1> colorAttachments = {
      AttachmentReferenceBuilder()
          .SetAttachment(0)
          .SetLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
          .BuildObject(),
  };
  const VkAttachmentReference depthStencilAttachment =
      AttachmentReferenceBuilder()
          .SetAttachment(1)
          .SetLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
          .BuildObject();
  const std::array<VkSubpassDescription, 1> subpasses {
      SubpassDescriptionBuilder()
          .SetPipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
          .SetColorAttachmentCount(colorAttachments.size())
          .SetPColorAttachments(colorAttachments.data())
          .SetPDepthStencilAttachment(&depthStencilAttachment)
          .BuildObject(),
  };
  const std::array<VkSubpassDependency, 1> subpassDependencies {
      SubpassDependencyBuilder()
          .SetSrcSubpass(VK_SUBPASS_EXTERNAL)
          .SetDstSubpass(0)
          .SetSrcStageMask(
              VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)
          .SetDstStageMask(
              VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)
          .SetSrcAccessMask(0)
          .SetDstAccessMask(
              VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT)
          .BuildObject(),
  };
  renderPass =
      virtualDevice.CreateRenderPass(
          RenderPassCreateInfoBuilder()
              .SetAttachmentCount(attachmentDescriptions.size())
              .SetPAttachments(attachmentDescriptions.data())
              .SetSubpassCount(subpasses.size())
              .SetPSubpasses(subpasses.data())
              .SetDependencyCount(subpassDependencies.size())
              .SetPDependencies(subpassDependencies.data()));
  swapchainFramebuffers = swapchain.GetFramebuffers(renderPass, depthStencilView);

  const std::array<VkVertexInputBindingDescription, 1> vertexBindingDescriptions {
      VertexInputBindingDescriptionBuilder()
          .SetBinding(0)
          .SetStride(sizeof(TexturedVertex))
          .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX)
          .BuildObject(),
  };
  const std::array<VkVertexInputAttributeDescription, 2> vertexAttributeDescriptions {
      VertexInputAttributeDescriptionBuilder()
          .SetBinding(0)
          .SetLocation(0)
          .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
          .SetOffset(offsetof(TexturedVertex, position))
          .BuildObject(),
      VertexInputAttributeDescriptionBuilder()
          .SetBinding(0)
          .SetLocation(1)
          .SetFormat(VK_FORMAT_R32G32_SFLOAT)
          .SetOffset(offsetof(TexturedVertex, textureCoordinate))
          .BuildObject(),
  };
  const std::array<VkDescriptorSetLayoutBinding, 2> descriptorSetLayoutBindings {
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
              .SetBindingCount(descriptorSetLayoutBindings.size())
              .SetPBindings(descriptorSetLayoutBindings.data()));
  pipeline =
      virtualDevice.CreateGraphicsPipeline(
          shaders,
          virtualDevice.CreatePipelineLayout(
              descriptorSetLayout,
              PipelineLayoutCreateInfoBuilder()
                  .SetPushConstantRangeCount(1)
                  .SetPPushConstantRanges(
                      PushConstantRangeBuilder()
                          .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT)
                          .SetOffset(0)
                          .SetSize(sizeof(ModelViewTransformation)))),
          SubpassReference(renderPass, 0),
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

  const std::array<VkVertexInputBindingDescription, 1> gradientVertexBindingDescriptions {
      VertexInputBindingDescriptionBuilder()
          .SetBinding(0)
          .SetStride(sizeof(GradientVertex))
          .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX)
          .BuildObject(),
  };
  const std::array<VkVertexInputAttributeDescription, 2> gradientVertexAttributeDescriptions {
      VertexInputAttributeDescriptionBuilder()
          .SetBinding(0)
          .SetLocation(0)
          .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
          .SetOffset(offsetof(GradientVertex, position))
          .BuildObject(),
      VertexInputAttributeDescriptionBuilder()
          .SetBinding(0)
          .SetLocation(1)
          .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
          .SetOffset(offsetof(GradientVertex, color))
          .BuildObject(),
  };
  const std::array<VkDescriptorSetLayoutBinding, 1> gradientDescriptorSetLayoutBindings {
      DescriptorSetLayoutBindingBuilder()
          .SetBinding(0)
          .SetDescriptorCount(1)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT)
          .BuildObject(),
  };
  gradientDescriptorSetLayout =
      virtualDevice.CreateDescriptorSetLayout(
          DescriptorSetLayoutCreateInfoBuilder()
              .SetBindingCount(gradientDescriptorSetLayoutBindings.size())
              .SetPBindings(gradientDescriptorSetLayoutBindings.data()));
  gradientPipeline =
      virtualDevice.CreateGraphicsPipeline(
          gradientShaders,
          virtualDevice.CreatePipelineLayout(
              gradientDescriptorSetLayout,
              PipelineLayoutCreateInfoBuilder()
                  .SetPushConstantRangeCount(1)
                  .SetPPushConstantRanges(
                      PushConstantRangeBuilder()
                          .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT)
                          .SetOffset(0)
                          .SetSize(sizeof(ModelViewTransformation)))),
          SubpassReference(renderPass, 0),
          GraphicsPipelineCreateInfoBuilder()
              .SetPDepthStencilState(
                  PipelineDepthStencilStateCreateInfoBuilder()
                      .SetDepthTestEnable(VK_TRUE)
                      .SetDepthWriteEnable(VK_TRUE)
                      .SetDepthCompareOp(VK_COMPARE_OP_LESS)
                      .SetDepthBoundsTestEnable(VK_FALSE))
              .SetPVertexInputState(
                  PipelineVertexInputStateCreateInfoBuilder()
                      .SetVertexBindingDescriptionCount(gradientVertexBindingDescriptions.size())
                      .SetPVertexBindingDescriptions(gradientVertexBindingDescriptions.data())
                      .SetVertexAttributeDescriptionCount(gradientVertexAttributeDescriptions.size())
                      .SetPVertexAttributeDescriptions(gradientVertexAttributeDescriptions.data()))
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
                              .SetBlendEnable(VK_TRUE)
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

  uiRenderer =
      UiRenderer(
          ImGuiInstance(
              windowInfo.window,
              instance,
              targetPhysicalDevice,
              virtualDevice,
              queue,
              renderPass,
              transientCommandBuffer,
              fence));

  const u32 swapchainImages = swapchain.GetImageCount();
  const std::array<VkDescriptorPoolSize, 2> descriptorPoolSizes {
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetDescriptorCount(swapchainImages * 2)
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
              .SetMaxSets(swapchainImages * 2));
  descriptorSets =
      descriptorPool.AllocateDescriptorSets(descriptorSetLayout, swapchainImages);
  gradientDescriptorSets =
      descriptorPool.AllocateDescriptorSets(gradientDescriptorSetLayout, swapchainImages);

  for (u32 renderIndex = 0; renderIndex < swapchainImages; ++renderIndex) {
    BufferWithMemory uniformBufferMemory =
        TransferDataToGpuLocalMemory(
            transientCommandBuffer,
            &projectionTransform,
            sizeof(projectionTransform),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

    DescriptorSet& descriptorSet = descriptorSets[renderIndex];
    const std::unique_ptr<DescriptorSet::WriteDescriptorSetBuild> bufferWrite =
        descriptorSet.CreateBufferWrite(uniformBufferMemory.buffer);
    const std::unique_ptr<DescriptorSet::WriteDescriptorSetBuild> textureSamplerWrite =
        descriptorSet.CreateImageSamplerWrite(textureView, textureSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    DescriptorSet& gradientDescriptorSet = gradientDescriptorSets[renderIndex];
    const std::unique_ptr<DescriptorSet::WriteDescriptorSetBuild> gradientBufferWrite =
        gradientDescriptorSet.CreateBufferWrite(uniformBufferMemory.buffer);
    std::array<VkWriteDescriptorSet, 3> descriptorSetWrites{
        bufferWrite->writeBuilder.BuildObject(),
        textureSamplerWrite->writeBuilder.SetDstBinding(1).BuildObject(),
        gradientBufferWrite->writeBuilder.BuildObject(),
    };
    virtualDevice.UpdateDescriptorSets(descriptorSetWrites.size(), descriptorSetWrites.data());

    CommandBuffer renderPassCommandBuffer = renderCommandPool.AllocatePrimaryCommandBuffer();

    swapchainRenderData.emplace_back(std::move<SwapchainRenderPass>({
      .renderData = std::move(uniformBufferMemory),
      .commandBuffer = std::move(renderPassCommandBuffer),
      .renderCompleteSemaphore = virtualDevice.CreateSemaphore(),
      .submitCompleteFence = virtualDevice.CreateFence(VK_FENCE_CREATE_SIGNALED_BIT)
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
    CommandBuffer& commandBuffer, const void* data, const u32 size, const VkBufferUsageFlags usage) {
  Buffer stagingBuffer =
      virtualDevice.CreateBuffer(
          BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
              .SetSize(size)
              .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  ReservedMemory stagingBufferMemory =
      deviceAllocator.BindMemory(stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.memoryBinding.GetMemory().MapCopy(data, stagingBufferMemory.offset, stagingBuffer.Size());

  Buffer finalBuffer =
      virtualDevice.CreateBuffer(
          BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
              .SetSize(size)
              .SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage));
  ReservedMemory finalBufferMemory =
      deviceAllocator.BindMemory(finalBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  commandBuffer.BeginOneTimeSubmit();
  commandBuffer.CmdCopyBufferFull(stagingBuffer, finalBuffer);
  commandBuffer.End().Submit(fence).Wait().Reset();

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
      uiRenderer.ProcessEvent(event);

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
//            acquiredImage.exists = false;
//            virtualDevice.WaitIdle();
//            swapchainRenderData.clear();
//            Log("Recreating swapchain.");
//            CommandPool temporaryCommandPool =
//                queue.CreateCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
//            CommandBuffer temporaryCommandBuffer = temporaryCommandPool.AllocatePrimaryCommandBuffer();
//            InitializeSwapchain(temporaryCommandBuffer);
//            Log("Swapchain recreated.");
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
  uiRenderer.BeginFrame();
  uiRenderer.ShowVulkanDebugInfo(VulkanDebugInfo{
      .gpuName = physicalDeviceProperties.deviceName,
      .frametime = deltaTime
  });
  uiRenderer.ShowKeyboardLayout(keyboard);

  constexpr glm::vec3 cameraStarePoint = glm::vec3(0.0f, -2.0f, -4.0f);
  glm::mat4 cubeTransform(1.0f);
  cubeTransform[3][0] = cubePosition.x;
  cubeTransform[3][1] = cubePosition.y;
  cubeTransform[3][2] = cubePosition.z;
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
  const glm::vec3 cameraCenter = glm::vec3(cameraRotatedCenter);

  renderTransform.model = rotatedCube;
  renderTransform.view = glm::lookAt(glm::vec3(0.0f), cameraCenter, glm::vec3(0.0f, 1.0f, 0.0f));

  glm::mat4 gradientCubeTransform(1.0f);
  gradientCubeTransform[3][0] = gradientCubePosition.x;
  gradientCubeTransform[3][1] = gradientCubePosition.y;
  gradientCubeTransform[3][2] = gradientCubePosition.z;
  gradientRenderTransform.model = glm::translate(gradientCubeTransform, cameraStarePoint);
  gradientRenderTransform.view = glm::lookAt(glm::vec3(0.0f), cameraCenter, glm::vec3(0.0f, 1.0f, 0.0f));

  std::vector<glm::vec3*> cubePositions { &cubePosition, &gradientCubePosition };
  u32 selectedObjectIndex = 0;
  uiRenderer.ShowObjectsInScene(ObjectsInSceneInfo{
      .cameraRotation = glm::vec2(cameraRotation.x, cameraRotation.y - 26.565f),
      .cameraCenter = glm::vec3(cameraCenter.x, cameraCenter.y - cameraStarePoint.y, cameraCenter.z - cameraStarePoint.z),
      .cameraPosition = glm::vec3(cameraStarePoint.x, -cameraStarePoint.y, -cameraStarePoint.z),
      .cubePositions = cubePositions,
      .selectedObjectIndex = &selectedObjectIndex
  });

  constexpr float movementSpeed = 10.0f;
  if (keyboard.IsKeyDown(SDLK_a)) {
    cubePositions[selectedObjectIndex]->x -= deltaTime * movementSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_d)) {
    cubePositions[selectedObjectIndex]->x += deltaTime * movementSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_w)) {
    cubePositions[selectedObjectIndex]->y += deltaTime * movementSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_s)) {
    cubePositions[selectedObjectIndex]->y -= deltaTime * movementSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_q)) {
    cubePositions[selectedObjectIndex]->z -= deltaTime * movementSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_e)) {
    cubePositions[selectedObjectIndex]->z += deltaTime * movementSpeed;
  }

  constexpr float cameraRotationSpeed = 75.0f;
  if (keyboard.IsKeyDown(SDLK_LEFT)) {
    cameraRotation.x += deltaTime * cameraRotationSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_RIGHT)) {
    cameraRotation.x -= deltaTime * cameraRotationSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_UP)) {
    cameraRotation.y += deltaTime * cameraRotationSpeed;
  }
  if (keyboard.IsKeyDown(SDLK_DOWN)) {
    cameraRotation.y -= deltaTime * cameraRotationSpeed;
  }

  cubeRotation += deltaTime * 90.0f;

  uiRenderer.EndFrame();
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
//      acquiredImage.exists = true;
//      acquiredImage.index = imageIndex;
      // Log("A");
      // This semaphore no longer has a way to be signalled
      // synchronisation.acquireImageSemaphore = virtualDevice.MakeSemaphore();
      // return;
    }
  }

  SwapchainRenderPass& swapchainRender = swapchainRenderData[imageIndex];

  swapchainRender.submitCompleteFence.Wait().Reset();
  {
    const std::array<VkClearValue, 2> clearValues{
        ClearValueBuilder()
            .SetColor(ClearColorValueBuilder().SetFloat0(0.2f).SetFloat1(0.2f).SetFloat2(0.2f).SetFloat3(1.0f))
            .BuildObject(),
        ClearValueBuilder()
            .SetDepthStencil(ClearDepthStencilValueBuilder().SetDepth(1.0f))
            .BuildObject()
    };
    
    swapchainRender.commandBuffer.Begin();
    swapchainRender.commandBuffer.CmdBeginRenderPass(
        RenderPassBeginInfoBuilder()
            .SetRenderArea(Rect2DBuilder().SetExtent(swapchain.GetImageExtent()))
            .SetClearValueCount(clearValues.size())
            .SetPClearValues(clearValues.data()),
        VK_SUBPASS_CONTENTS_INLINE,
        renderPass,
        swapchainFramebuffers[imageIndex]);
    swapchainRender.commandBuffer.CmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    swapchainRender.commandBuffer.CmdBindVertexBuffers(vertexMemoryBuffer.buffer, 0);
    swapchainRender.commandBuffer.CmdBindIndexBuffer(indexMemoryBuffer.buffer, VK_INDEX_TYPE_UINT16);
    swapchainRender.commandBuffer.CmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetLayout(), descriptorSets[0]);
    swapchainRender.commandBuffer.CmdPushConstants(pipeline.GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(renderTransform), &renderTransform);
    swapchainRender.commandBuffer.CmdDrawIndexed(indexCount, /* instanceCount= */ 1); // TODO: More instances
    swapchainRender.commandBuffer.CmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, gradientPipeline);
    swapchainRender.commandBuffer.CmdBindVertexBuffers(gradientVertexMemoryBuffer.buffer, 0);
    swapchainRender.commandBuffer.CmdBindIndexBuffer(gradientIndexMemoryBuffer.buffer, VK_INDEX_TYPE_UINT16);
    swapchainRender.commandBuffer.CmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, gradientPipeline.GetLayout(), gradientDescriptorSets[0]);
    swapchainRender.commandBuffer.CmdPushConstants(gradientPipeline.GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(gradientRenderTransform), &gradientRenderTransform);
    swapchainRender.commandBuffer.CmdDrawIndexed(gradientIndexCount, /* instanceCount= */ 1); // TODO: More instances
    uiRenderer.Render(swapchainRender.commandBuffer);
    swapchainRender.commandBuffer.CmdEndRenderPass();
    swapchainRender.commandBuffer.End();
  }

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
    // Log("B");
    // return;
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
