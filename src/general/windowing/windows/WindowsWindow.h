#ifndef VULKAN_SRC_GENERAL_WINDOWING_WINDOWS_WINDOWSWINDOW_H
#define VULKAN_SRC_GENERAL_WINDOWING_WINDOWS_WINDOWSWINDOW_H

#include "util/build_definition.h"

#if defined(WINDOWS)

#include "general/windowing/Window.h"
#include "util/include/windows.h"

class WindowsWindow : public wnd::Window {
 public:
  WindowsWindow(const Recti windowRect, SDL_Window* window,
                HINSTANCE instanceHandle, HWND windowHandle);

  Surface CreateWindowSurface(VkInstance instance) const override;

 private:
  HINSTANCE instanceHandle;
  HWND windowHandle;
};

#endif  // defined(WINDOWS)

#endif  // VULKAN_SRC_GENERAL_WINDOWING_WINDOWS_WINDOWSWINDOW_H
