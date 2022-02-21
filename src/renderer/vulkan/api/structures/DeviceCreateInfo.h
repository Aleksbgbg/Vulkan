#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DEVICECREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DEVICECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "PhysicalDeviceFeatures.h"
#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(DeviceCreateInfoBuilder, VkDeviceCreateInfo,
                  VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO)
STRUCTURE_SETTER(QueueCreateInfoCount, const u32, queueCreateInfoCount)
STRUCTURE_SETTER(PQueueCreateInfos, const VkDeviceQueueCreateInfo*,
                 pQueueCreateInfos)
STRUCTURE_SETTER(EnabledLayerCount, const u32, enabledLayerCount)
STRUCTURE_SETTER(PpEnabledLayerNames, const char* const*, ppEnabledLayerNames)
STRUCTURE_SETTER(EnabledExtensionCount, const u32, enabledExtensionCount)
STRUCTURE_SETTER(PpEnabledExtensionNames, const char* const*,
                 ppEnabledExtensionNames)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PEnabledFeatures,
                                      PhysicalDeviceFeaturesBuilder,
                                      pEnabledFeatures)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DEVICECREATEINFO_H_
