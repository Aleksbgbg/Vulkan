#include "DebugUtilsMessenger.h"

#include <utility>

#include "error.h"

DebugUtilsMessenger::DebugUtilsMessenger() : messenger(nullptr) {}

DebugUtilsMessenger::DebugUtilsMessenger(
    VkInstance instance,
    const DebugUtilsMessengerCreateInfoExtBuilder& infoBuilder)
    : instance(instance) {
  PROCEED_ON_VALID_RESULT(vkCreateDebugUtilsMessengerEXT(
      instance, infoBuilder.Build(), nullptr, &messenger));
}

DebugUtilsMessenger::DebugUtilsMessenger(DebugUtilsMessenger&& other) noexcept
    : instance(other.instance), messenger(other.messenger) {
  other.messenger = nullptr;
}

DebugUtilsMessenger::~DebugUtilsMessenger() {
  if (messenger != nullptr) {
    vkDeleteDebugUtilsMessengerEXT(instance, messenger, nullptr);
  }
}

DebugUtilsMessenger& DebugUtilsMessenger::operator=(
    DebugUtilsMessenger&& other) noexcept {
  std::swap(instance, other.instance);
  std::swap(messenger, other.messenger);
  return *this;
}

VkResult DebugUtilsMessenger::vkCreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger) {
  auto function = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

  if (function == nullptr) {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }

  return function(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void DebugUtilsMessenger::vkDeleteDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks* pAllocator) {
  auto function = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

  return function(instance, messenger, pAllocator);
}
