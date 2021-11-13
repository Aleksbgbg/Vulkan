#ifndef VULKAN_SRC_GENERAL_WINDOWING_LINUX_LINUXWINDOW_H
#define VULKAN_SRC_GENERAL_WINDOWING_LINUX_LINUXWINDOW_H

#include "util/build_definition.h"

#if defined(LINUX)

#include "general/windowing/Window.h"
#include "util/include/sdl.h"

// Messes with other includes
#include <X11/Xlib.h>

class LinuxWindow : public wnd::Window {
 public:
  LinuxWindow(const Recti windowRect, SDL_Window* sdlWindow, Display* display,
              ::Window window);

  Surface CreateWindowSurface(VkInstance instance) const override;

 private:
  Display* display;
  ::Window window;
};

#endif  // defined(LINUX)

#endif  // VULKAN_SRC_GENERAL_WINDOWING_LINUX_LINUXWINDOW_H
