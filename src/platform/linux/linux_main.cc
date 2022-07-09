// Must come first to import the correct SDL (otherwise SDL.h in other files
// refers to SDL1)
#include <SDL2/SDL.h>

#include "renderer/vulkan/api/error.h"
#include "run.h"
#include "system/System.h"
#include "system/windowing/Window.h"
#include "util/include/sdl.h"

// Must come last or they break other includes
#include <SDL2/SDL_syswm.h>
#include <X11/Xlib.h>

#include "XlibSurfaceCreateInfo.h"

class LinuxWindow : public sys::Window {
 public:
  LinuxWindow(const Recti windowRect, SDL_Window* sdlWindow, Display* display,
              ::Window window)
      : Window(windowRect, sdlWindow), display_(display), window_(window) {}

  vk::Surface CreateWindowSurface(VkInstance instance) const {
    VkSurfaceKHR surface;
    PROCEED_ON_VALID_RESULT(
        vkCreateXlibSurfaceKHR(instance,
                               XlibSurfaceCreateInfoBuilder()
                                   .SetDpy(display_)
                                   .SetWindow(window_)
                                   .Build(),
                               nullptr, &surface));
    return vk::Surface(instance, surface);
  }

 private:
  Display* display_;
  ::Window window_;
};

class LinuxSystem : public sys::System {
 public:
  const char* SurfaceExtensionName() const override {
    return VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
  }

 private:
  std::unique_ptr<sys::Window> CreateWindow(
      SDL_Window* const window, const Recti windowRect) const override {
    SDL_SysWMinfo sysWmInfo;
    SDL_VERSION(&sysWmInfo.version)
    SDL_GetWindowWMInfo(window, &sysWmInfo);

    return std::make_unique<LinuxWindow>(windowRect, window,
                                         sysWmInfo.info.x11.display,
                                         sysWmInfo.info.x11.window);
  }
};

int main() {
  LinuxSystem system;
  return run(system);
}
