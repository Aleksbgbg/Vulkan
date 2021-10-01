#ifndef VULKAN_SRC_APP_H
#define VULKAN_SRC_APP_H

#include <chrono>
#include <vector>
#include <semaphore>

#include <SDL_syswm.h>
#include <vulkan/vulkan.h>
#include <vulkan/Swapchain.h>

#include "vulkan/VulkanInstance.h"
#include "vulkan/Semaphore.h"
#include "vulkan/structures/ApplicationInfo.h"
#include "vulkan/ImGuiInstance.h"
#include "Keyboard.h"
#include "SdlIncl.h"
#include "GlmIncl.h"

// Bad windows headers
#include <vulkan/vulkan_win32.h>
#undef PostMessage

#include "MultithreadedMessageQueue.h"
#include "UiRenderer.h"

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
    DeviceMemory memory;
  };

  struct ImageWithMemory {
    Image image;
    DeviceMemory memory;
  };

  struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
  };

  BufferWithMemory TransferDataToGpuLocalMemory(
      CommandBuffer& commandBuffer, const void* data, const u32 size, const VkBufferUsageFlags usage) const;

  VkFormat SelectDepthStencilFormat(const std::vector<VkFormat>& potentialFormats) const;

  VkSurfaceFormatKHR SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats) const;
  static VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR surfaceCapabilities);
  static VkPresentModeKHR SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
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

  BufferWithMemory vertexMemoryBuffer;
  BufferWithMemory indexMemoryBuffer;
  u32 indexCount;

  ImageWithMemory texture;
  ImageView textureView;
  Sampler textureSampler;

  RenderPass renderPass;

  CommandPool renderCommandPool;
  std::vector<ShaderModule> shaders;
  Swapchain swapchain;

  ImageWithMemory depthStencil;
  ImageView depthStencilView;
  DescriptorSetLayout descriptorSetLayout;
  Pipeline pipeline;
  std::vector<Framebuffer> swapchainFramebuffers;

  DescriptorPool descriptorPool;

  VkPhysicalDeviceProperties physicalDeviceProperties;
  UiRenderer uiRenderer;

  UniformBufferObject renderTransform;

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

  enum class EventNotification {
    Exited,
    Paused,
    Unpaused,
    Resized
  };
  MultithreadedMessageQueue<EventNotification> threadMessenger;

  struct AcquiredImage {
    bool exists;
    u32 index;
  };
  AcquiredImage acquiredImage;

  std::chrono::time_point<std::chrono::steady_clock> previousTime;

  float cubeRotation = 0.0f;
  glm::vec3 cubePosition;

  glm::vec2 cameraRotation;

  Keyboard keyboard;

  std::vector<DescriptorSet> descriptorSets;
};

#endif // VULKAN_SRC_APP_H
