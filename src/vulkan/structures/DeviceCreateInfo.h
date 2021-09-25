#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DEVICECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DEVICECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"
#include "PhysicalDeviceFeatures.h"

STRUCTURE_BUILDER(DeviceCreateInfoBuilder, VkDeviceCreateInfo, VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO)
  STRUCTURE_SETTER(DeviceCreateInfoBuilder, QueueCreateInfoCount, const u32, queueCreateInfoCount)
  STRUCTURE_SETTER(DeviceCreateInfoBuilder, PQueueCreateInfos, const VkDeviceQueueCreateInfo*, pQueueCreateInfos)
  STRUCTURE_SETTER(DeviceCreateInfoBuilder, EnabledLayerCount, const u32, enabledLayerCount)
  STRUCTURE_SETTER(DeviceCreateInfoBuilder, PpEnabledLayerNames, const char* const*, ppEnabledLayerNames)
  STRUCTURE_SETTER(DeviceCreateInfoBuilder, EnabledExtensionCount, const u32, enabledExtensionCount)
  STRUCTURE_SETTER(DeviceCreateInfoBuilder, PpEnabledExtensionNames, const char* const*, ppEnabledExtensionNames)
  STRUCTURE_SETTER_BUILDER(DeviceCreateInfoBuilder, PEnabledFeatures, PhysicalDeviceFeaturesBuilder, pEnabledFeatures)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_DEVICECREATEINFO_H
