#include "Window.h"

#include <backends/imgui_impl_sdl.h>

Window::WindowInfo Window::InitSdl(const u32 width, const u32 height) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  Recti windowRect = Recti::FromRegion(50, 50, width, height);

  SDL_Window* window = SDL_CreateWindow(
      "Vulkan", windowRect.X(), windowRect.Y(), windowRect.Width(),
      windowRect.Height(),
      SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  SDL_SysWMinfo sysWmInfo;
  SDL_VERSION(&sysWmInfo.version)
  SDL_GetWindowWMInfo(window, &sysWmInfo);

  return WindowInfo{.window = window,
                    .hinstance = sysWmInfo.info.win.hinstance,
                    .hwnd = sysWmInfo.info.win.window,
                    .rect = windowRect};
}

Window::Window(const u32 width, const u32 height)
    : windowInfo(InitSdl(width, height)), keyboard(), mouse() {}

Window::~Window() {
  SDL_Quit();
}

Recti Window::GetRect() const {
  return windowInfo.rect;
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

  ImGui_ImplSDL2_ProcessEvent(&event);

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
          windowInfo.rect =
              Rectf::FromRegion(windowInfo.rect.X(), windowInfo.rect.Y(),
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

Surface Window::CreateWindowSurface(const VulkanInstance& instance) const {
  return instance.CreateSurface(Win32SurfaceCreateInfoBuilder()
                                    .SetHinstance(windowInfo.hinstance)
                                    .SetHwnd(windowInfo.hwnd));
}

void Window::InitImGui() const {
  ImGui_ImplSDL2_InitForVulkan(windowInfo.window);
}

void Window::NewFrame() const {
  ImGui_ImplSDL2_NewFrame(windowInfo.window);
}

void Window::ShutdownImGui() const {
  ImGui_ImplSDL2_Shutdown();
}
