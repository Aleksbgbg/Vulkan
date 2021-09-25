#ifndef VULKAN_SRC_VULKAN_DEBUGUTILSMESSENGER_H
#define VULKAN_SRC_VULKAN_DEBUGUTILSMESSENGER_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/DebugUtilsMessengerCreateInfo.h"

class DebugUtilsMessenger {
public:
  VULKAN_OBJECT_MOVABLE_ROOT(DebugUtilsMessenger, instance, messenger)

  DebugUtilsMessenger() = default;
  DebugUtilsMessenger(VkInstance instance, DebugUtilsMessengerCreateInfoExtBuilder& infoBuilder);

  DebugUtilsMessenger(const DebugUtilsMessenger&) = delete;

  ~DebugUtilsMessenger();

  DebugUtilsMessenger& operator=(const DebugUtilsMessenger&) = delete;

private:
  static VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                 const VkAllocationCallbacks* pAllocator,
                                                 VkDebugUtilsMessengerEXT* pDebugMessenger);
  static void vkDeleteDebugUtilsMessengerEXT(VkInstance instance,
                                             VkDebugUtilsMessengerEXT messenger,
                                             const VkAllocationCallbacks* pAllocator);

private:
  VkInstance instance;
  VkDebugUtilsMessengerEXT messenger = nullptr;
};

#endif // VULKAN_SRC_VULKAN_DEBUGUTILSMESSENGER_H
