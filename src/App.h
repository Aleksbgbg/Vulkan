#ifndef VULKAN_SRC_APP_H
#define VULKAN_SRC_APP_H

#include <vulkan/vulkan.h>

#include <chrono>
#include <vector>

#include "DynamicUniformBuffer.h"
#include "UiRenderer.h"
#include "general/animations/NormalizedOneTimeFunctionAnimation.h"
#include "general/animations/UpAndDownGlidingAnimation.h"
#include "general/geometry/Rect.h"
#include "general/threading/MultithreadedMessageQueue.h"
#include "general/windowing/Window.h"
#include "general/windowing/input/Keyboard.h"
#include "general/windowing/input/Mouse.h"
#include "memory/DeviceMemoryAllocator.h"
#include "util/include/glm.h"
#include "vulkan/ImGuiInstance.h"
#include "vulkan/Swapchain.h"
#include "vulkan/VulkanInstance.h"

class App {
 public:
  App();
  ~App();

  int Run();

 private:
  void MainThread();
  void RenderThread();

  void MainLoop();

  void UpdateModel(const float deltaTime);
  void Render();

  void InitializeSwapchain();

 private:
  struct BufferWithMemory {
    Buffer buffer;
    DeviceMemorySubAllocation memory;
  };

  struct ImageWithMemory {
    Image image;
    DeviceMemorySubAllocation memory;
  };

  struct PerSceneData {
    alignas(16) glm::mat4 projection;
  };

  struct PerFrameData {
    alignas(16) glm::mat4 view;
  };

  struct PerObjectData {
    alignas(16) glm::mat4 model;
  };

  BufferWithMemory TransferDataToGpuLocalMemory(CommandBuffer& commandBuffer,
                                                const void* data,
                                                const u32 size,
                                                const VkBufferUsageFlags usage);

  VkFormat SelectDepthStencilFormat(
      const std::vector<VkFormat>& potentialFormats) const;

  VkSurfaceFormatKHR SelectSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats) const;
  static VkExtent2D SelectSwapExtent(
      const VkSurfaceCapabilitiesKHR surfaceCapabilities);
  static VkPresentModeKHR SelectSwapPresentMode(
      const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkSampleCountFlagBits SelectMsaaSamples(
      const VkSampleCountFlagBits preferred);

  static VKAPI_ATTR VkBool32 VKAPI_CALL
  DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);

  Window window;
  const Keyboard& keyboard;
  const Mouse& mouse;

  VulkanInstance instance;
  DebugUtilsMessenger debugMessenger;
  Surface windowSurface;
  u32 minSwapchainImages;
  u32 framesInFlight;

  PhysicalDevice targetPhysicalDevice;
  VkPhysicalDeviceProperties physicalDeviceProperties;

  VirtualDevice virtualDevice;
  Queue queue;

  PipelineCache pipelineCache;

  DeviceMemoryAllocator deviceAllocator;

  Fence fence;

  CommandPool shortExecutionCommandPool;
  CommandBuffer shortExecutionCommandBuffer;

  BufferWithMemory vertexMemoryBuffer;
  BufferWithMemory indexMemoryBuffer;

  bool moving;
  u32 stationaryIndexCount;
  u32 movingIndexCount;

  ImageWithMemory texture;
  ImageView textureView;
  Sampler textureSampler;

  RenderPass renderPass;

  CommandPool renderCommandPool;
  std::vector<ShaderModule> shaders;

  bool hasSwapchain;
  Swapchain swapchain;
  bool hasOldSwapchain;
  Swapchain oldSwapchain;

  ImageWithMemory depthStencil;
  ImageView depthStencilView;

  ImageWithMemory multisamplingImage;
  ImageView multisamplingImageView;

  std::vector<Framebuffer> swapchainFramebuffers;

  DescriptorSetLayout perSceneDescriptorSetLayout;
  DescriptorSetLayout perFrameDescriptorSetLayout;
  DescriptorSetLayout textureSamplerDescriptorSetLayout;

  Pipeline pipeline;

  DescriptorPool descriptorPool;
  DescriptorSet sceneDescriptorSet;
  DescriptorSet textureSamplerDescriptorSet;

  DynamicUniformBuffer<PerFrameData> viewTransformBuffer;

  std::unique_ptr<UiRenderer> uiRenderer;

  glm::vec3 modelCenter;
  glm::vec3 modelSize;
  glm::vec3 modelPosition;
  PerObjectData modelTransform;

  BufferWithMemory projectionTransformBuffer;
  PerSceneData projectionTransform;

  struct SwapchainRenderPass {
    CommandBuffer commandBuffer;

    Semaphore renderCompleteSemaphore;
    Fence submitCompleteFence;
  };
  std::vector<SwapchainRenderPass> swapchainRenderData;
  struct InFlightImage {
    Semaphore acquireImageSemaphore;
  };
  std::vector<InFlightImage> imagesInFlightSynchronisation;
  u32 currentInFlightImage = 0;

  enum class EventNotification { Exited, Paused, Unpaused, Resized };
  MultithreadedMessageQueue<EventNotification> threadMessenger;

  std::chrono::time_point<std::chrono::steady_clock> previousTime;
};

#endif  // VULKAN_SRC_APP_H
