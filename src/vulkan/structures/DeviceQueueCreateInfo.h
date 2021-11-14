#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DEVICEQUEUECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DEVICEQUEUECREATEINFO_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(DeviceQueueCreateInfoBuilder, VkDeviceQueueCreateInfo,
                  VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO)
STRUCTURE_SETTER(QueueFamilyIndex, const u32, queueFamilyIndex)
STRUCTURE_SETTER(QueueCount, const u32, queueCount)
STRUCTURE_SETTER(PQueuePriorities, const float*, pQueuePriorities)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DEVICEQUEUECREATEINFO_H
