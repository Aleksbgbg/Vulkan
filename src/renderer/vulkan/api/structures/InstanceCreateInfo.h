#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_INSTANCECREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_INSTANCECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "ApplicationInfo.h"
#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(InstanceCreateInfoBuilder, VkInstanceCreateInfo,
                  VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO)
STRUCTURE_SETTER(PApplicationInfo, const VkApplicationInfo* const,
                 pApplicationInfo)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(ApplicationInfo, ApplicationInfoBuilder,
                                      pApplicationInfo)
STRUCTURE_SETTER(EnabledLayerCount, const u32, enabledLayerCount)
STRUCTURE_SETTER(PpEnabledLayerNames, const char* const* const,
                 ppEnabledLayerNames)
STRUCTURE_SETTER(EnabledExtensionCount, const u32, enabledExtensionCount)
STRUCTURE_SETTER(PpEnabledExtensionNames, const char* const* const,
                 ppEnabledExtensionNames)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_INSTANCECREATEINFO_H_
