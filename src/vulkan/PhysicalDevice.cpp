#include "PhysicalDevice.h"

#include "util.h"
#include "error.h"

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physicalDevice) : physicalDevice(physicalDevice) {
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
}

VkPhysicalDeviceProperties PhysicalDevice::GetProperties() const {
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(physicalDevice, &properties);
  return properties;
}

VirtualDevice PhysicalDevice::CreateVirtualDevice(DeviceCreateInfoBuilder& infoBuilder) {
  return VirtualDevice(physicalDevice, &memoryProperties, infoBuilder);
}

std::optional<u32> PhysicalDevice::FindAppropriateQueueFamily(
    const VkQueueFlags requiredProperties, const std::function<bool(u32)>& predicate) const {
  std::vector<VkQueueFamilyProperties> queues = LoadArray([this](u32* count, VkQueueFamilyProperties* properties) {
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, count, properties);
  });

  for (u32 queueIndex = 0; queueIndex < queues.size(); ++queueIndex) {
    VkQueueFamilyProperties queueProperties = queues[queueIndex];

    if ((queueProperties.queueFlags & requiredProperties) == requiredProperties) {
      if (predicate(queueIndex)) {
        return queueIndex;
      }
    }
  }

  return std::nullopt;
}

bool PhysicalDevice::SupportsSurface(const u32 queueFamilyIndex, VkSurfaceKHR surface) const {
  VkBool32 presentSupport = VK_FALSE;
  PROCEED_ON_VALID_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, &presentSupport))
  return presentSupport == VK_TRUE;
}

VkSurfaceCapabilitiesKHR PhysicalDevice::GetCapabilities(VkSurfaceKHR surface) const {
  VkSurfaceCapabilitiesKHR capabilities;
  PROCEED_ON_VALID_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities))
  return capabilities;
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::GetFormats(VkSurfaceKHR surface) const {
  return LoadArray([this, surface](u32* const count, VkSurfaceFormatKHR* const formats) {
    PROCEED_ON_VALID_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, count, formats))
  });
}

std::vector<VkPresentModeKHR> PhysicalDevice::GetPresentModes(VkSurfaceKHR surface) const {
  return LoadArray([this, surface](u32* const count, VkPresentModeKHR* const presentModes) {
    PROCEED_ON_VALID_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, count, presentModes))
  });
}

VkFormatProperties PhysicalDevice::GetFormatProperties(const VkFormat format) const {
  VkFormatProperties formatProperties;
  vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);
  return formatProperties;
}

VkImageFormatProperties PhysicalDevice::GetSurfaceImageFormatProperties(const VkSurfaceFormatKHR& surfaceFormat) const {
  VkImageFormatProperties properties;
  PROCEED_ON_VALID_RESULT(
      vkGetPhysicalDeviceImageFormatProperties(
          physicalDevice,
          surfaceFormat.format,
          VK_IMAGE_TYPE_2D,
          VK_IMAGE_TILING_OPTIMAL,
          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
          0,
          &properties))
  return properties;
}

const VkPhysicalDeviceMemoryProperties* PhysicalDevice::GetMemoryProperties() const {
  return &memoryProperties;
}
