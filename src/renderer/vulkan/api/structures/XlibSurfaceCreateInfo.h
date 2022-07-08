#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_XLIBSURFACECREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_XLIBSURFACECREATEINFO_H_

#include <X11/Xlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>

#include "define_structure.h"

STRUCTURE_BUILDER(XlibSurfaceCreateInfoBuilder, VkXlibSurfaceCreateInfoKHR,
                  VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR)
STRUCTURE_SETTER(Flags, VkXlibSurfaceCreateFlagsKHR, flags)
STRUCTURE_SETTER(Dpy, Display*, dpy)
STRUCTURE_SETTER(Window, Window, window)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_XLIBSURFACECREATEINFO_H_
