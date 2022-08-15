#ifndef SRC_APP_H_
#define SRC_APP_H_

#include "core/clock.h"
#include "core/text/FontAtlas.h"
#include "core/threading/MultithreadedMessageQueue.h"
#include "engine/RenderPerformanceTracker.h"
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
  RenderPerformanceTracker renderPerformanceTracker_;

  Controls controls_;
  game::Camera camera_;
  Scene scene_;

  enum class EventNotification { Exited, Paused, Unpaused, Resized };
  MultithreadedMessageQueue<EventNotification> threadMessenger_;

  time_point previousTime_;
};

#endif  // SRC_APP_H_
