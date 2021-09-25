#ifndef VULKAN_SRC_VULKAN_STRUCTURES_INSTANCECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_INSTANCECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"
#include "ApplicationInfo.h"

STRUCTURE_BUILDER(InstanceCreateInfoBuilder, VkInstanceCreateInfo, VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO)
  STRUCTURE_SETTER(InstanceCreateInfoBuilder, PApplicationInfo, const VkApplicationInfo* const, pApplicationInfo)
  STRUCTURE_SETTER_BUILDER(InstanceCreateInfoBuilder, ApplicationInfo, ApplicationInfoBuilder, pApplicationInfo)
  STRUCTURE_SETTER(InstanceCreateInfoBuilder, EnabledLayerCount, const u32, enabledLayerCount)
  STRUCTURE_SETTER(InstanceCreateInfoBuilder, PpEnabledLayerNames, const char* const* const, ppEnabledLayerNames)
  STRUCTURE_SETTER(InstanceCreateInfoBuilder, EnabledExtensionCount, const u32, enabledExtensionCount)
  STRUCTURE_SETTER(InstanceCreateInfoBuilder, PpEnabledExtensionNames, const char* const* const, ppEnabledExtensionNames)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_INSTANCECREATEINFO_H
