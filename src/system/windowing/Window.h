#ifndef VULKAN_SRC_SYSTEM_WINDOWING_WINDOW_H_
#define VULKAN_SRC_SYSTEM_WINDOWING_WINDOW_H_

#include "general/geometry/Rect.h"
#include "system/windowing/input/Keyboard.h"
#include "system/windowing/input/Mouse.h"
#include "vulkan/SurfaceFactory.h"

namespace sys {

class Window : public SurfaceFactory {
 public:
  enum class Event {
    None,
    Exit,
    Minimized,
    Restored,
    SizeChanged,
  };

  Window(const Recti windowRect, SDL_Window* window);

  Window(const Window&) = delete;
  Window(Window&& other) noexcept;

  virtual ~Window();

  Window& operator=(const Window&) = delete;
  Window& operator=(Window&& other) noexcept;

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

}  // namespace sys

#endif  // VULKAN_SRC_SYSTEM_WINDOWING_WINDOW_H_
