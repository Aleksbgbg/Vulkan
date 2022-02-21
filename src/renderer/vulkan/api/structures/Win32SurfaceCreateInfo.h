#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_WIN32SURFACECREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_WIN32SURFACECREATEINFO_H_

// Must come before vulkan
#include "util/include/windows.h"
// Must come after windows.h
#include <vulkan/vulkan_win32.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(Win32SurfaceCreateInfoBuilder, VkWin32SurfaceCreateInfoKHR,
                  VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR)
STRUCTURE_SETTER(Hinstance, HINSTANCE, hinstance)
STRUCTURE_SETTER(Hwnd, HWND, hwnd)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_WIN32SURFACECREATEINFO_H_
