#ifndef VULKAN_SRC_APP_H
#define VULKAN_SRC_APP_H

#include <vulkan/vulkan.h>

#include <chrono>
#include <vector>

#include "Keyboard.h"
#include "MultithreadedMessageQueue.h"
#include "UiRenderer.h"
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

  void InitializeSwapchain(CommandBuffer& transientCommandBuffer);

 private:
  struct Rect {
    glm::ivec2 position;
    glm::ivec2 size;
  };

  struct WindowInfo {
    SDL_Window* window;
    HINSTANCE hinstance;
    HWND hwnd;
    Rect rect;
  };

  struct BufferWithMemory {
    Buffer buffer;
    ReservedMemory memory;
  };

  struct ImageWithMemory {
    Image image;
    ReservedMemory memory;
  };

  struct ModelViewTransformation {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
  };

  struct ProjectionTransformation {
    alignas(16) glm::mat4 value;
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

  DeviceMemoryAllocator deviceAllocator;

  Fence fence;

  BufferWithMemory vertexMemoryBuffer;
  BufferWithMemory indexMemoryBuffer;
  u32 indexCount;

  BufferWithMemory gradientVertexMemoryBuffer;
  BufferWithMemory gradientIndexMemoryBuffer;
  u32 gradientIndexCount;

  ImageWithMemory texture;
  ImageView textureView;
  Sampler textureSampler;

  RenderPass renderPass;

  CommandPool renderCommandPool;
  std::vector<ShaderModule> shaders;
  std::vector<ShaderModule> gradientShaders;
  Swapchain swapchain;

  ImageWithMemory depthStencil;
  ImageView depthStencilView;
  std::vector<Framebuffer> swapchainFramebuffers;

  DescriptorSetLayout descriptorSetLayout;
  Pipeline pipeline;

  DescriptorSetLayout gradientDescriptorSetLayout;
  Pipeline gradientPipeline;

  DescriptorPool descriptorPool;

  VkPhysicalDeviceProperties physicalDeviceProperties;
  UiRenderer uiRenderer;

  ModelViewTransformation gradientRenderTransform;

  ModelViewTransformation renderTransform;
  ProjectionTransformation projectionTransform;

  struct SwapchainRenderPass {
    BufferWithMemory renderData;
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

  struct AcquiredImage {
    bool exists;
    u32 index;
  };
  AcquiredImage acquiredImage;

  std::chrono::time_point<std::chrono::steady_clock> previousTime;

  float cubeRotation = 0.0f;
  glm::vec3 cubePosition;

  glm::vec3 gradientCubePosition;

  glm::vec2 cameraRotation;

  Keyboard keyboard;

  std::vector<DescriptorSet> descriptorSets;
  std::vector<DescriptorSet> gradientDescriptorSets;
};

#endif  // VULKAN_SRC_APP_H
