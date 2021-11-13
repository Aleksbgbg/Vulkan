#ifndef VULKAN_SRC_GENERAL_WINDOWING_SYSTEM_WINDOW_H
#define VULKAN_SRC_GENERAL_WINDOWING_SYSTEM_WINDOW_H

#include "Window.h"
#include "util/types.h"

std::unique_ptr<wnd::Window> InitializeSystemWindow(const u32 width,
                                                    const u32 height);

#endif  // VULKAN_SRC_GENERAL_WINDOWING_SYSTEM_WINDOW_H
