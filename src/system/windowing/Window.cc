#include "Window.h"

namespace sys {

Window::Window(const Recti windowRect, SDL_Window* window)
    : isFocused_(false),
      windowRect_(windowRect),
      window_(window),
      keyboard_(),
      mouse_(),
      windowEventConsumers_() {}

Window::Window(Window&& other) noexcept
    : windowRect_(other.windowRect_),
      window_(other.window_),
      keyboard_(std::move(other.keyboard_)),
      mouse_(std::move(other.mouse_)),
      windowEventConsumers_(std::move(other.windowEventConsumers_)) {
  other.window_ = nullptr;
}

Window& Window::operator=(Window&& other) noexcept {
  windowRect_ = other.windowRect_;
  std::swap(window_, other.window_);
  keyboard_ = std::move(other.keyboard_);
  mouse_ = std::move(other.mouse_);
  windowEventConsumers_ = std::move(other.windowEventConsumers_);
  return *this;
}

Window::~Window() {
  if (window_ != nullptr) {
    SDL_Quit();
  }
}

void Window::Consume(WindowEventConsumer& consumer) {
  windowEventConsumers_.push_back(&consumer);
}

bool Window::IsFocused() const {
  return isFocused_;
}

glm::vec2 Window::GetSize() const {
  return windowRect_.Size();
}

Recti Window::GetRect() const {
  return windowRect_;
}

const Keyboard& Window::GetKeyboard() const {
  return keyboard_;
}

const Mouse& Window::GetMouse() const {
  return mouse_;
}

Window::Event Window::WaitAndProcessEvent() {
  SDL_Event event;
  SDL_WaitEvent(&event);

  for (WindowEventConsumer* consumer : windowEventConsumers_) {
    consumer->Consume(event);
  }

  switch (event.type) {
    case SDL_QUIT:
      return Event::Exit;

    case SDL_WINDOWEVENT:
      switch (event.window.event) {
        case SDL_WINDOWEVENT_MINIMIZED:
          return Event::Minimized;

        case SDL_WINDOWEVENT_RESTORED:
          return Event::Restored;

        case SDL_WINDOWEVENT_SIZE_CHANGED:
          windowRect_ =
              Rectf::FromRegion(windowRect_.X(), windowRect_.Y(),
                                event.window.data1, event.window.data2);
          return Event::SizeChanged;

        case SDL_WINDOWEVENT_FOCUS_GAINED:
          isFocused_ = true;
          break;

        case SDL_WINDOWEVENT_FOCUS_LOST:
          isFocused_ = false;
          break;
      }
      break;

    case SDL_KEYDOWN:
      if (event.key.repeat == 0) {
        keyboard_.KeyDown(event.key.keysym.sym);
      }
      break;

    case SDL_KEYUP:
      keyboard_.KeyUp(event.key.keysym.sym);
      break;

    case SDL_MOUSEMOTION:
      mouse_.Movement(event.motion);
      break;
  }

  return Event::None;
}

void Window::ClearInputs() {
  keyboard_.ClearPressedKeys();
}

}  // namespace sys
