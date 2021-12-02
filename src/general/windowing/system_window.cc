#include "system_window.h"

#include "util/build_definition.h"

#if defined(WINDOWS)

// Required at the top to disable windows features
#include <SDL_syswm.h>

#include "general/windowing/windows/WindowsWindow.h"
#include "util/include/sdl.h"
#include "util/include/windows.h"

std::unique_ptr<wnd::Window> CreateWindow(SDL_Window* window,
                                          SDL_SysWMinfo sysWmInfo,
                                          Recti windowRect) {
  return std::make_unique<WindowsWindow>(windowRect, window,
                                         sysWmInfo.info.win.hinstance,
                                         sysWmInfo.info.win.window);
}

#elif defined(LINUX)

#undef SDL_VIDEO_DRIVER_WINDOWS
#define SDL_VIDEO_DRIVER_X11
#include <SDL_syswm.h>

#include "general/windowing/linux/LinuxWindow.h"

std::unique_ptr<wnd::Window> CreateWindow(SDL_Window* window,
                                          SDL_SysWMinfo sysWmInfo,
                                          Recti windowRect) {
  return std::make_unique<LinuxWindow>(windowRect, window,
                                       sysWmInfo.info.x11.display,
                                       sysWmInfo.info.x11.window);
}

#endif

std::unique_ptr<wnd::Window> InitializeSystemWindow(const u32 width,
                                                    const u32 height) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  const Recti windowRect = Recti::FromRegion(50, 50, width, height);

  SDL_Window* window = SDL_CreateWindow(
      "Vulkan", windowRect.X(), windowRect.Y(), windowRect.Width(),
      windowRect.Height(),
      SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  SDL_SysWMinfo sysWmInfo;
  SDL_VERSION(&sysWmInfo.version)
  SDL_GetWindowWMInfo(window, &sysWmInfo);

  return CreateWindow(window, sysWmInfo, windowRect);
}
