#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_APPLICATIONINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_APPLICATIONINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(ApplicationInfoBuilder, VkApplicationInfo,
                  VK_STRUCTURE_TYPE_APPLICATION_INFO)
STRUCTURE_SETTER(PApplicationName, const char*, pApplicationName)
STRUCTURE_SETTER(ApplicationVersion, const i32, applicationVersion)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_APPLICATIONINFO_H_
