#include "Window.h"

#include <backends/imgui_impl_sdl.h>

namespace wnd {

Window::Window(const Recti windowRect, SDL_Window* window)
    : windowRect(windowRect), window(window), keyboard(), mouse() {}

Window::Window(Window&& other) noexcept
    : windowRect(other.windowRect),
      window(other.window),
      keyboard(std::move(other.keyboard)),
      mouse(std::move(other.mouse)) {
  other.window = nullptr;
}

Window& Window::operator=(Window&& other) noexcept {
  windowRect = other.windowRect;
  std::swap(window, other.window);
  keyboard = std::move(other.keyboard);
  mouse = std::move(other.mouse);

  return *this;
}

Window::~Window() {
  if (window != nullptr) {
    SDL_Quit();
  }
}

Recti Window::GetRect() const {
  return windowRect;
}

Keyboard& Window::GetKeyboard() {
  return keyboard;
}

const Keyboard& Window::GetKeyboard() const {
  return keyboard;
}

const Mouse& Window::GetMouse() const {
  return mouse;
}

Window::Event Window::WaitAndProcessEvent() {
  SDL_Event event;
  SDL_WaitEvent(&event);

  // ImGui_ImplSDL2_ProcessEvent(&event);

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
          windowRect =
              Rectf::FromRegion(windowRect.X(), windowRect.Y(),
                                event.window.data1, event.window.data2);
          return Event::SizeChanged;
      }
      break;

    case SDL_KEYDOWN:
      if (event.key.repeat == 0) {
        keyboard.Keydown(event.key.keysym.sym);
      }
      break;

    case SDL_KEYUP:
      keyboard.Keyup(event.key.keysym.sym);
      break;

    case SDL_MOUSEMOTION:
      mouse.Movement(event.motion);
      break;

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      mouse.Button(event.button);
      break;
  }

  return Event::None;
}

void Window::EndFrame() {
  keyboard.ClearPressedKeys();
}

void Window::InitImGui() const {
  //  ImGui_ImplSDL2_InitForVulkan(window);
}

void Window::NewFrame() const {
  //  ImGui_ImplSDL2_NewFrame(window);
}

void Window::ShutdownImGui() const {
  //  ImGui_ImplSDL2_Shutdown();
}

}  // namespace wnd
