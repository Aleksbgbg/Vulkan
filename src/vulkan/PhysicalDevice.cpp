#include "PhysicalDevice.h"

#include "error.h"
#include "util.h"

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physicalDevice)
    : physicalDevice(physicalDevice) {
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
}

VkPhysicalDeviceProperties PhysicalDevice::GetProperties() const {
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(physicalDevice, &properties);
  return properties;
}

VirtualDevice PhysicalDevice::CreateVirtualDevice(
    DeviceCreateInfoBuilder& infoBuilder) {
  return VirtualDevice(physicalDevice, infoBuilder);
}

std::optional<u32> PhysicalDevice::FindAppropriateQueueFamily(
    const VkQueueFlags requiredProperties,
    const std::function<bool(u32)>& predicate) const {
  std::vector<VkQueueFamilyProperties> queues =
      LoadArray([this](u32* count, VkQueueFamilyProperties* properties) {
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, count,
                                                 properties);
      });

  for (u32 queueIndex = 0; queueIndex < queues.size(); ++queueIndex) {
    VkQueueFamilyProperties queueProperties = queues[queueIndex];

    if ((queueProperties.queueFlags & requiredProperties) ==
        requiredProperties) {
      if (predicate(queueIndex)) {
        return queueIndex;
      }
    }
  }

  return std::nullopt;
}

bool PhysicalDevice::SupportsSurface(const u32 queueFamilyIndex,
                                     VkSurfaceKHR surface) const {
  VkBool32 presentSupport = VK_FALSE;
  PROCEED_ON_VALID_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(
      physicalDevice, queueFamilyIndex, surface, &presentSupport))
  return presentSupport == VK_TRUE;
}

VkSurfaceCapabilitiesKHR PhysicalDevice::GetCapabilities(
    VkSurfaceKHR surface) const {
  VkSurfaceCapabilitiesKHR capabilities;
  PROCEED_ON_VALID_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      physicalDevice, surface, &capabilities))
  return capabilities;
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::GetFormats(
    VkSurfaceKHR surface) const {
  return LoadArray(
      [this, surface](u32* const count, VkSurfaceFormatKHR* const formats) {
        PROCEED_ON_VALID_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevice, surface, count, formats))
      });
}

std::vector<VkPresentModeKHR> PhysicalDevice::GetPresentModes(
    VkSurfaceKHR surface) const {
  return LoadArray(
      [this, surface](u32* const count, VkPresentModeKHR* const presentModes) {
        PROCEED_ON_VALID_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevice, surface, count, presentModes))
      });
}

VkFormatProperties PhysicalDevice::GetFormatProperties(
    const VkFormat format) const {
  VkFormatProperties formatProperties;
  vkGetPhysicalDeviceFormatProperties(physicalDevice, format,
                                      &formatProperties);
  return formatProperties;
}

const VkPhysicalDeviceMemoryProperties* PhysicalDevice::GetMemoryProperties()
    const {
  return &memoryProperties;
}
