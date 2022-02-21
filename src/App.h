#ifndef VULKAN_SRC_APP_H_
#define VULKAN_SRC_APP_H_

#include <chrono>

#include "core/text/FontAtlas.h"
#include "core/threading/MultithreadedMessageQueue.h"
#include "game/Scene.h"
#include "renderer/vulkan/Vulkan.h"
#include "system/sound/Sound.h"
#include "system/windowing/Window.h"

class App {
 public:
  App(sys::Window& window, sys::Sound& sound, Vulkan& vulkan,
      const FontAtlas& fontAtlas, Settings& settings);

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
