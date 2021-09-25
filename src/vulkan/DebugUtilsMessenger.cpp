#include "DebugUtilsMessenger.h"

#include "error.h"

DebugUtilsMessenger::DebugUtilsMessenger(VkInstance instance, DebugUtilsMessengerCreateInfoExtBuilder& infoBuilder)
  :
  instance(instance) {
  PROCEED_ON_VALID_RESULT(vkCreateDebugUtilsMessengerEXT(instance, infoBuilder.Build(), nullptr, &messenger))
}

DebugUtilsMessenger::~DebugUtilsMessenger() {
  if (messenger != nullptr) {
    vkDeleteDebugUtilsMessengerEXT(instance, messenger, nullptr);
  }
}

VkResult DebugUtilsMessenger::vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                                             const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                             const VkAllocationCallbacks* pAllocator,
                                                             VkDebugUtilsMessengerEXT* pDebugMessenger) {
  auto function =
      reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

  if (function == nullptr) {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }

  return function(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void DebugUtilsMessenger::vkDeleteDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger,
                                                         const VkAllocationCallbacks* pAllocator) {
  auto function =
      reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

  return function(instance, messenger, pAllocator);
}
