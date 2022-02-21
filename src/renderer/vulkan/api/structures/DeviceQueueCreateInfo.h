#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DEVICEQUEUECREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DEVICEQUEUECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(DeviceQueueCreateInfoBuilder, VkDeviceQueueCreateInfo,
                  VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO)
STRUCTURE_SETTER(QueueFamilyIndex, const u32, queueFamilyIndex)
STRUCTURE_SETTER(QueueCount, const u32, queueCount)
STRUCTURE_SETTER(PQueuePriorities, const float*, pQueuePriorities)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DEVICEQUEUECREATEINFO_H_
