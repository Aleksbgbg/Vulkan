#ifndef VULKAN_SRC_GENERAL_WINDOWING_WINDOW_H
#define VULKAN_SRC_GENERAL_WINDOWING_WINDOW_H

#include "general/geometry/Rect.h"
#include "general/windowing/ImGuiWindow.h"
#include "general/windowing/input/Keyboard.h"
#include "general/windowing/input/Mouse.h"
#include "util/include/sdl.h"
#include "util/types.h"
#include "vulkan/Surface.h"
#include "vulkan/VulkanInstance.h"

class Window : public ImGuiWindow {
 public:
  Window(const u32 width, const u32 height);

  ~Window();

 public:
  enum class Event {
    None,
    Exit,
    Minimized,
    Restored,
    SizeChanged,
  };

  Recti GetRect() const;

  Keyboard& GetKeyboard();

  const Keyboard& GetKeyboard() const;
  const Mouse& GetMouse() const;

  Event WaitAndProcessEvent();

  void EndFrame();

  Surface CreateWindowSurface(const VulkanInstance& instance) const;

 private:
  void InitImGui() const override;
  void NewFrame() const override;

 public:
  void ShutdownImGui() const override;

 private:
  struct WindowInfo {
    SDL_Window* window;
    HINSTANCE hinstance;
    HWND hwnd;
    Recti rect;
  };

  static WindowInfo InitSdl(const u32 width, const u32 height);

 private:
  WindowInfo windowInfo;

  Keyboard keyboard;
  Mouse mouse;
};

#endif  // VULKAN_SRC_GENERAL_WINDOWING_WINDOW_H
