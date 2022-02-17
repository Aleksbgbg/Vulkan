#include <SDL.h>
#include <SDL_syswm.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_macos.h>

#include "renderer/vulkan/api/Surface.h"
#include "renderer/vulkan/api/error.h"
#include "run.h"
#include "system/System.h"
#include "system/windowing/Window.h"
#include "util/include/sdl.h"

class MacWindow : public sys::Window {
 public:
  MacWindow(const Recti windowRect, SDL_Window* sdlWindow)
      : Window(windowRect, sdlWindow), sdlWindow_(sdlWindow) {}

  vk::Surface CreateWindowSurface(VkInstance instance) const {
    VkSurfaceKHR surface;
    SDL_Vulkan_CreateSurface(sdlWindow_, instance, &surface);
    return vk::Surface(instance, surface);
  }

 private:
  SDL_Window* sdlWindow_;
};

class MacSystem : public sys::System {
 public:
  const char* SurfaceExtensionName() const override {
    return VK_MVK_MACOS_SURFACE_EXTENSION_NAME;
  }

 private:
  std::unique_ptr<sys::Window> CreateWindow(
      SDL_Window* const window, const Recti windowRect) const override {
    SDL_SysWMinfo sysWmInfo;
    SDL_VERSION(&sysWmInfo.version)
    SDL_GetWindowWMInfo(window, &sysWmInfo);

    return std::make_unique<MacWindow>(windowRect, window);
  }
};

int main() {
  MacSystem system;
  return run(system);
}
