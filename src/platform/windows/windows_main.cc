#include "Win32SurfaceCreateInfo.h"
#include "renderer/vulkan/api/error.h"
#include "run.h"
#include "system/System.h"
#include "system/windowing/Window.h"
#include "util/include/sdl.h"
#include "windows.h"

// Must come after sdl.h and windows.h
#include "SDL_syswm.h"

class WindowsWindow : public sys::Window {
 public:
  WindowsWindow(const Recti windowRect, SDL_Window* window,
                HINSTANCE instanceHandle, HWND windowHandle)
      : Window(windowRect, window),
        instanceHandle_(instanceHandle),
        windowHandle_(windowHandle) {}

  vk::Surface CreateWindowSurface(VkInstance vulkanInstance) const {
    VkSurfaceKHR surface;
    PROCEED_ON_VALID_RESULT(
        vkCreateWin32SurfaceKHR(vulkanInstance,
                                Win32SurfaceCreateInfoBuilder()
                                    .SetHinstance(instanceHandle_)
                                    .SetHwnd(windowHandle_)
                                    .Build(),
                                nullptr, &surface));
    return vk::Surface(vulkanInstance, surface);
  }

 private:
  HINSTANCE instanceHandle_;
  HWND windowHandle_;
};

class WindowsSystem : public sys::System {
 public:
  const char* SurfaceExtensionName() const override {
    return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
  }

 private:
  std::unique_ptr<sys::Window> CreateWindow(
      SDL_Window* const window, const Recti windowRect) const override {
    SDL_SysWMinfo sysWmInfo;
    SDL_VERSION(&sysWmInfo.version)
    SDL_GetWindowWMInfo(window, &sysWmInfo);

    return std::make_unique<WindowsWindow>(windowRect, window,
                                           sysWmInfo.info.win.hinstance,
                                           sysWmInfo.info.win.window);
  }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine,
                   const int nCmdShow) {
  WindowsSystem system;
  return run(system);
}
