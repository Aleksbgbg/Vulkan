#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_DEBUGUTILSMESSENGERCREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_DEBUGUTILSMESSENGERCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

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

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_DEBUGUTILSMESSENGERCREATEINFO_H_
