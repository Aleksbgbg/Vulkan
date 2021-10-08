#ifndef VULKAN_SRC_VULKAN_STRUCTURES_WIN32SURFACECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_WIN32SURFACECREATEINFO_H

#include <vulkan/vulkan_win32.h>

#include "Windows.h"
#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(Win32SurfaceCreateInfoBuilder, VkWin32SurfaceCreateInfoKHR,
                  VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR)
STRUCTURE_SETTER(Win32SurfaceCreateInfoBuilder, Hinstance, HINSTANCE, hinstance)
STRUCTURE_SETTER(Win32SurfaceCreateInfoBuilder, Hwnd, HWND, hwnd)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_WIN32SURFACECREATEINFO_H
