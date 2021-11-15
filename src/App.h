#ifndef VULKAN_SRC_APP_H
#define VULKAN_SRC_APP_H

#include <vulkan/vulkan.h>

#include <chrono>
#include <vector>

#include "DynamicUniformBuffer.h"
#include "SwapchainWithResources.h"
#include "UiRenderer.h"
#include "game/ResourceAllocator.h"
#include "game/Scene.h"
#include "general/animations/NormalizedOneTimeFunctionAnimation.h"
#include "general/geometry/Rect.h"
#include "general/threading/MultithreadedMessageQueue.h"
#include "general/windowing/Window.h"
#include "general/windowing/input/Keyboard.h"
#include "general/windowing/input/Mouse.h"
#include "memory/DeviceMemoryAllocator.h"
#include "vulkan/ImGuiInstance.h"
#include "vulkan/Swapchain.h"
#include "vulkan/VulkanInstance.h"

class App {
 public:
  App(std::unique_ptr<wnd::Window> appWindow);
  ~App();

  int Run();

 private:
  void MainThread();
  void RenderThread();

  void MainLoop();

  void UpdateModel(const float deltaTime);
  void Render();

 private:
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

  std::unique_ptr<wnd::Window> window;
  const Keyboard& keyboard;

  VulkanInstance instance;
  DebugUtilsMessenger debugMessenger;
  Surface windowSurface;
  u32 minSwapchainImages;

  PhysicalDevice targetPhysicalDevice;
  VkPhysicalDeviceProperties physicalDeviceProperties;

  VirtualDevice virtualDevice;
  Queue queue;

  PipelineCache pipelineCache;

  DeviceMemoryAllocator deviceAllocator;

  Fence fence;

  CommandPool shortExecutionCommandPool;
  CommandBuffer shortExecutionCommandBuffer;

  VkSurfaceFormatKHR surfaceFormat;
  VkFormat depthStencilFormat;
  VkSampleCountFlagBits samples;

  ResourceAllocator resourceAllocator;

  std::unique_ptr<Scene> scene;

  Sampler textureSampler;

  RenderPass renderPass;

  CommandPool renderCommandPool;

  SwapchainWithResources swapchain;
  SwapchainWithResources oldSwapchain;

  std::unique_ptr<UiRenderer> uiRenderer;

  struct SwapchainRenderPass {
    CommandBuffer commandBuffer;

    Semaphore renderCompleteSemaphore;
    Fence submitCompleteFence;
  };
  std::vector<SwapchainRenderPass> swapchainRenderData;

  u32 imageIndex;

  enum class EventNotification { Exited, Paused, Unpaused, Resized };
  MultithreadedMessageQueue<EventNotification> threadMessenger;

  std::chrono::time_point<std::chrono::high_resolution_clock> previousTime;
};

#endif  // VULKAN_SRC_APP_H
