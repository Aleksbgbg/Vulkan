#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DEVICEQUEUECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DEVICEQUEUECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(DeviceQueueCreateInfoBuilder, VkDeviceQueueCreateInfo, VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO)
  STRUCTURE_SETTER(DeviceQueueCreateInfoBuilder, QueueFamilyIndex, const u32, queueFamilyIndex)
  STRUCTURE_SETTER(DeviceQueueCreateInfoBuilder, QueueCount, const u32, queueCount)
  STRUCTURE_SETTER(DeviceQueueCreateInfoBuilder, PQueuePriorities, const float*, pQueuePriorities)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_DEVICEQUEUECREATEINFO_H
