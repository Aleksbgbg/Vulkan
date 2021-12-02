#include "WindowsWindow.h"

#if defined(WINDOWS)

#include <vulkan/vulkan.h>

#include "vulkan/error.h"
#include "vulkan/structures/Win32SurfaceCreateInfo.h"

WindowsWindow::WindowsWindow(const Recti windowRect, SDL_Window* window,
                             HINSTANCE instanceHandle, HWND windowHandle)
    : Window(windowRect, window),
      instanceHandle(instanceHandle),
      windowHandle(windowHandle) {}

Surface WindowsWindow::CreateWindowSurface(VkInstance vulkanInstance) const {
  VkSurfaceKHR surface;
  PROCEED_ON_VALID_RESULT(
      vkCreateWin32SurfaceKHR(vulkanInstance,
                              Win32SurfaceCreateInfoBuilder()
                                  .SetHinstance(instanceHandle)
                                  .SetHwnd(windowHandle)
                                  .Build(),
                              nullptr, &surface));
  return Surface(vulkanInstance, surface);
}

#endif  // defined(WINDOWS)
