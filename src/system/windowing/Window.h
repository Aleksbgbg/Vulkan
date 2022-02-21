#ifndef VULKAN_SRC_SYSTEM_WINDOWING_WINDOW_H_
#define VULKAN_SRC_SYSTEM_WINDOWING_WINDOW_H_

#include <vector>

#include "WindowEventConsumerConsumer.h"
#include "core/math/Rect.h"
#include "renderer/vulkan/api/SurfaceFactory.h"
#include "system/windowing/input/Keyboard.h"
#include "system/windowing/input/Mouse.h"

namespace sys {

class Window : public vk::SurfaceFactory, public WindowEventConsumerConsumer {
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

  void Consume(WindowEventConsumer& consumer) override;

  bool IsFocused() const;

  glm::vec2 GetSize() const;
  Recti GetRect() const;

  const Keyboard& GetKeyboard() const;
  const Mouse& GetMouse() const;

  Event WaitAndProcessEvent();

  void ClearInputs();

 private:
  bool isFocused_;
  Recti windowRect_;
  SDL_Window* window_;
  Keyboard keyboard_;
  Mouse mouse_;
  std::vector<WindowEventConsumer*> windowEventConsumers_;
};

}  // namespace sys

#endif  // VULKAN_SRC_SYSTEM_WINDOWING_WINDOW_H_
