#include "App.h"

#include <array>
#include <thread>

#include "general/logging/log.h"

App::App(sys::Window& window, sys::Sound& sound, Vulkan& vulkan)
    : window_(window),
      vulkan_(vulkan),
      controls_(),
      camera_(),
      scene_(vulkan_, sound, camera_),
      previousTime_(std::chrono::high_resolution_clock::time_point::min()),
      threadMessenger_() {}

int App::Run() {
  std::thread renderThread(&App::RenderThread, this);
  MainThread();
  renderThread.join();
  return 0;
}

void App::MainThread() {
  while (true) {
    switch (window_.WaitAndProcessEvent()) {
      case sys::Window::Event::Exit:
        threadMessenger_.PostMessage(EventNotification::Unpaused);
        threadMessenger_.PostMessage(EventNotification::Exited);
        return;

      case sys::Window::Event::Minimized:
        threadMessenger_.PostMessage(EventNotification::Paused);
        break;

      case sys::Window::Event::Restored:
        threadMessenger_.PostMessage(EventNotification::Unpaused);
        break;

      case sys::Window::Event::SizeChanged:
        threadMessenger_.PostMessage(EventNotification::Resized);
        break;
    }
  }
}

void App::RenderThread() {
  try {
    while (true) {
      MainLoop();

      while (threadMessenger_.HasMessage()) {
        EventNotification message = threadMessenger_.PopMessage();

        switch (message) {
          case EventNotification::Exited:
            return;

          case EventNotification::Paused:
            threadMessenger_.WaitMessage(EventNotification::Unpaused);
            break;

          case EventNotification::Resized:
            vulkan_.WindowResized();
            break;
        }
      }
    }
  } catch (const std::exception& exception) {
    ImmediateLog(exception.what());
    throw;
  }
}

void App::MainLoop() {
  if (previousTime_ == std::chrono::high_resolution_clock::time_point::min()) {
    previousTime_ = std::chrono::high_resolution_clock::now();
  }

  const auto timeNow = std::chrono::high_resolution_clock::now();
  const float deltaTime =
      std::chrono::duration<float, std::chrono::seconds::period>(timeNow -
                                                                 previousTime_)
          .count();

  UpdateModel(deltaTime);
  Render();

  previousTime_ = timeNow;
}

void App::UpdateModel(const float deltaTime) {
  controls_.Update(window_);
  window_.ClearInputs();

  const UpdateContext updateContext{.deltaTime = deltaTime,
                                    .controls = controls_};
  vulkan_.ScheduleCompute({.deltaTime = deltaTime});
  scene_.UpdateModel(updateContext);
}

void App::Render() {
  vulkan_.ScheduleRender(camera_, window_);
}
