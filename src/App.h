#ifndef VULKAN_SRC_APP_H
#define VULKAN_SRC_APP_H

#include <vulkan/vulkan.h>

#include <chrono>
#include <vector>

#include "Keyboard.h"
#include "MultithreadedMessageQueue.h"
#include "NormalizedOneTimeFunctionAnimation.h"
#include "Rect.h"
#include "UiRenderer.h"
#include "UpAndDownGlidingAnimation.h"
#include "include_glm.h"
#include "memory/DeviceMemoryAllocator.h"
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
  struct WindowInfo {
    SDL_Window* window;
    HINSTANCE hinstance;
    HWND hwnd;
    Recti rect;
  };

  struct BufferWithMemory {
    Buffer buffer;
    ReservedMemory memory;
  };

  struct ImageWithMemory {
    Image image;
    ReservedMemory memory;
  };

  struct ModelTransformation {
    alignas(16) glm::mat4 model;
  };

  struct ViewTransformation {
    alignas(16) glm::mat4 view;
  };

  struct ProjectionTransformation {
    alignas(16) glm::mat4 value;
  };

  struct Highlight {
    alignas(4) float multiplier;
  };

  struct PushConstantLayout {
    ModelTransformation modelTransform;
    Highlight highlight;
  };

  void SpawnGradientCube();

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
  VkSampleCountFlagBits SelectMsaaSamples(const VkSampleCountFlagBits preferred);

  static VKAPI_ATTR VkBool32 VKAPI_CALL
  DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);

  static WindowInfo InitSdl();

  WindowInfo windowInfo;

  VulkanInstance instance;
  DebugUtilsMessenger debugMessenger;
  Surface windowSurface;
  u32 minSwapchainImages;
  u32 framesInFlight;

  PhysicalDevice targetPhysicalDevice;
  VirtualDevice virtualDevice;
  Queue queue;

  PipelineCache pipelineCache;

  DeviceMemoryAllocator deviceAllocator;

  Fence fence;

  CommandPool shortExecutionCommandPool;
  CommandBuffer shortExecutionCommandBuffer;

  BufferWithMemory vertexMemoryBuffer;
  BufferWithMemory indexMemoryBuffer;
  u32 indexCount;

  ModelTransformation modelTransform;
  Highlight highlight;

  BufferWithMemory gradientVertexMemoryBuffer;
  BufferWithMemory gradientIndexMemoryBuffer;
  u32 gradientIndexCount;

  struct GradientCubeInstance {
    glm::vec3 position;
    ModelTransformation modelTransform;
    Highlight highlight;

    NormalizedOneTimeFunctionAnimation spawnAnimation;
  };
  std::vector<GradientCubeInstance> gradientCubes;

  ImageWithMemory texture;
  ImageView textureView;
  Sampler textureSampler;

  RenderPass renderPass;

  CommandPool renderCommandPool;
  std::vector<ShaderModule> shaders;
  std::vector<ShaderModule> gradientShaders;

  bool hasSwapchain;
  Swapchain swapchain;
  bool hasOldSwapchain;
  Swapchain oldSwapchain;

  ImageWithMemory depthStencil;
  ImageView depthStencilView;

  ImageWithMemory multisamplingImage;
  ImageView multisamplingImageView;

  std::vector<Framebuffer> swapchainFramebuffers;

  DescriptorSetLayout projectionViewDescriptorSetLayout;
  DescriptorSetLayout textureSamplerDescriptorSetLayout;

  Pipeline pipeline;
  Pipeline gradientPipeline;

  DescriptorPool descriptorPool;
  std::vector<DescriptorSet> projectionViewDescriptorSets;
  std::vector<DescriptorSet> textureSamplerDescriptorSets;

  VkPhysicalDeviceProperties physicalDeviceProperties;

  std::unique_ptr<UiRenderer> uiRenderer;

  ViewTransformation viewTransform;
  ProjectionTransformation projectionTransform;

  BufferWithMemory projectionTransformBuffer;

  struct SwapchainRenderPass {
    CommandBuffer commandBuffer;

    BufferWithMemory viewTransformCpuBuffer;
    BufferWithMemory viewTransformGpuBuffer;

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

  float cubeRotation = 0.0f;
  glm::vec3 cubePosition;

  glm::vec2 cameraRotation;

  Keyboard keyboard;

  UpAndDownGlidingAnimation highlightAnimation;
  u32 lastSelectedObjectIndex;
};

#endif  // VULKAN_SRC_APP_H
