#ifndef VULKAN_SRC_RENDERER_VULKAN_API_DEBUGUTILSMESSENGER_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_DEBUGUTILSMESSENGER_H_

#include <vulkan/vulkan.h>

#include "renderer/vulkan/api/structures/DebugUtilsMessengerCreateInfo.h"

namespace vk {

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

}  // namespace vk

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_DEBUGUTILSMESSENGER_H_
