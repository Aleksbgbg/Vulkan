#include "LinuxWindow.h"

#if defined(LINUX)

#include "vulkan/error.h"
#include "vulkan/structures/XlibSurfaceCreateInfo.h"

LinuxWindow::LinuxWindow(const Recti windowRect, SDL_Window* sdlWindow,
                         Display* display, ::Window window)
    : Window(windowRect, sdlWindow), display(display), window(window) {}

Surface LinuxWindow::CreateWindowSurface(VkInstance instance) const {
  VkSurfaceKHR surface;
  PROCEED_ON_VALID_RESULT(vkCreateXlibSurfaceKHR(
      instance,
      XlibSurfaceCreateInfoBuilder().SetDpy(display).SetWindow(window).Build(),
      nullptr, &surface));
  return Surface(instance, surface);
}

#endif  // defined(LINUX)
