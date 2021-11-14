#ifndef VULKAN_SRC_VULKAN_DEBUGUTILSMESSENGER_H
#define VULKAN_SRC_VULKAN_DEBUGUTILSMESSENGER_H

#include <vulkan/vulkan.h>

#include "structures/DebugUtilsMessengerCreateInfo.h"

class DebugUtilsMessenger {
 public:
  DebugUtilsMessenger();
  DebugUtilsMessenger(
      VkInstance instance,
      const DebugUtilsMessengerCreateInfoExtBuilder& infoBuilder);

  DebugUtilsMessenger(const DebugUtilsMessenger&) = delete;
  DebugUtilsMessenger(DebugUtilsMessenger&& other) noexcept;

  ~DebugUtilsMessenger();

  DebugUtilsMessenger& operator=(const DebugUtilsMessenger&) = delete;
  DebugUtilsMessenger& operator=(DebugUtilsMessenger&& other) noexcept;

 private:
  static VkResult vkCreateDebugUtilsMessengerEXT(
      VkInstance instance,
      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
      const VkAllocationCallbacks* pAllocator,
      VkDebugUtilsMessengerEXT* pDebugMessenger);
  static void vkDeleteDebugUtilsMessengerEXT(
      VkInstance instance, VkDebugUtilsMessengerEXT messenger,
      const VkAllocationCallbacks* pAllocator);

 private:
  VkInstance instance;
  VkDebugUtilsMessengerEXT messenger;
};

#endif  // VULKAN_SRC_VULKAN_DEBUGUTILSMESSENGER_H
