#ifndef VULKAN_SRC_APP_H_
#define VULKAN_SRC_APP_H_

#include <chrono>

#include "game/Scene.h"
#include "general/threading/MultithreadedMessageQueue.h"
#include "renderer/vulkan/Vulkan.h"
#include "system/windowing/Window.h"

class App {
 public:
  App(sys::Window& window, Vulkan& vulkan);

  int Run();

 private:
  void MainThread();
  void RenderThread();

  void MainLoop();

  void UpdateModel(const float deltaTime);
  void Render();

 private:
  sys::Window& window_;
  Vulkan& vulkan_;

  Controls controls_;
  game::Camera camera_;
  Scene scene_;

  enum class EventNotification { Exited, Paused, Unpaused, Resized };
  MultithreadedMessageQueue<EventNotification> threadMessenger_;

  std::chrono::time_point<std::chrono::high_resolution_clock> previousTime_;
};

#endif  // VULKAN_SRC_APP_H_
