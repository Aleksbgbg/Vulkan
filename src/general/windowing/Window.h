#ifndef VULKAN_SRC_GENERAL_WINDOWING_WINDOW_H_
#define VULKAN_SRC_GENERAL_WINDOWING_WINDOW_H_

#include "VulkanWindow.h"
#include "general/geometry/Rect.h"
#include "general/windowing/input/Keyboard.h"
#include "general/windowing/input/Mouse.h"
#include "util/include/sdl.h"

namespace wnd {

class Window : public VulkanWindow {
 public:
  Window(const Recti windowRect, SDL_Window* window);

  Window(const Window&) = delete;
  Window(Window&& other) noexcept;

  virtual ~Window();

  Window& operator=(const Window&) = delete;
  Window& operator=(Window&& other) noexcept;

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

 private:
  Recti windowRect;
  SDL_Window* window;
  Keyboard keyboard;
  Mouse mouse;
};

}  // namespace wnd

#endif  // VULKAN_SRC_GENERAL_WINDOWING_WINDOW_H_
