#ifndef VULKAN_SRC_APP_H
#define VULKAN_SRC_APP_H

#include <chrono>
#include <memory>
#include <vector>

#include "AppNetwork.h"
#include "SwapchainWithResources.h"
#include "Vulkan.h"
#include "game/Scene.h"
#include "general/threading/MultithreadedMessageQueue.h"
#include "general/windowing/Window.h"
#include "util/types.h"
#include "vulkan/CommandBuffer.h"
#include "vulkan/Fence.h"
#include "vulkan/Semaphore.h"

class App {
 public:
  App(wnd::Window& window, std::unique_ptr<Vulkan> vulkan,
      std::unique_ptr<AppNetwork> appNetwork);
  ~App();

  int Run();

 private:
  void MainThread();
  void RenderThread();

  void MainLoop();

  void UpdateModel(const float deltaTime);
  void Render();

 private:
  wnd::Window& window_;

  std::unique_ptr<AppNetwork> appNetwork_;
  Controls controls_;
  NetworkedPlayerController mainPlayerController_;

  std::unique_ptr<Vulkan> vulkan_;

  std::unique_ptr<Scene> scene_;

  SwapchainWithResources swapchain_;
  SwapchainWithResources oldSwapchain_;

  u32 imageIndex_;

  enum class EventNotification { Exited, Paused, Unpaused, Resized };
  MultithreadedMessageQueue<EventNotification> threadMessenger_;

  std::chrono::time_point<std::chrono::high_resolution_clock> previousTime_;

  struct SwapchainRenderPass {
    CommandBuffer commandBuffer;

    Semaphore renderCompleteSemaphore;
    Fence submitCompleteFence;
  };
  std::vector<SwapchainRenderPass> swapchainRenderData_;
};

#endif  // VULKAN_SRC_APP_H
