#ifndef VULKAN_SRC_VULKAN_STRUCTURES_INSTANCECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_INSTANCECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "ApplicationInfo.h"
#include "define_structure.h"
#include "util/types.h"

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

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_INSTANCECREATEINFO_H
