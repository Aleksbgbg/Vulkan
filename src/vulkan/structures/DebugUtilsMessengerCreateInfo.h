#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DEBUGUTILSMESSENGERCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DEBUGUTILSMESSENGERCREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(DebugUtilsMessengerCreateInfoExtBuilder,
                  VkDebugUtilsMessengerCreateInfoEXT,
                  VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT)
STRUCTURE_SETTER(DebugUtilsMessengerCreateInfoExtBuilder, MessageSeverity,
                 const VkDebugUtilsMessageSeverityFlagsEXT, messageSeverity)
STRUCTURE_SETTER(DebugUtilsMessengerCreateInfoExtBuilder, MessageType,
                 const VkDebugUtilsMessageTypeFlagsEXT, messageType)
STRUCTURE_SETTER(DebugUtilsMessengerCreateInfoExtBuilder, PfnUserCallback,
                 PFN_vkDebugUtilsMessengerCallbackEXT, pfnUserCallback)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DEBUGUTILSMESSENGERCREATEINFO_H
