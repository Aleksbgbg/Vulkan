#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DEBUGUTILSMESSENGERCREATEINFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_DEBUGUTILSMESSENGERCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(DebugUtilsMessengerCreateInfoExtBuilder,
                  VkDebugUtilsMessengerCreateInfoEXT,
                  VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT)
STRUCTURE_SETTER(MessageSeverity, const VkDebugUtilsMessageSeverityFlagsEXT,
                 messageSeverity)
STRUCTURE_SETTER(MessageType, const VkDebugUtilsMessageTypeFlagsEXT,
                 messageType)
STRUCTURE_SETTER(PfnUserCallback, PFN_vkDebugUtilsMessengerCallbackEXT,
                 pfnUserCallback)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DEBUGUTILSMESSENGERCREATEINFO_H_
