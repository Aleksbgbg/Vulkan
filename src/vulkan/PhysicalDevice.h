#ifndef VULKAN_SRC_VULKAN_PHYSICALDEVICE_H
#define VULKAN_SRC_VULKAN_PHYSICALDEVICE_H

#include <functional>
#include <optional>
#include <string_view>

#include <vulkan/vulkan.h>

#include "structures/DeviceCreateInfo.h"
#include "VirtualDevice.h"
#include "Queue.h"

class PhysicalDevice {
public:
  PhysicalDevice() = default;
  explicit PhysicalDevice(VkPhysicalDevice physicalDevice);

  VkPhysicalDeviceProperties GetProperties() const;
  std::optional<u32> FindAppropriateQueueFamily(const VkQueueFlags requiredProperties,
                                                const std::function<bool(u32)>& predicate) const;
  VirtualDevice CreateVirtualDevice(DeviceCreateInfoBuilder& infoBuilder);
  bool SupportsSurface(const u32 queueFamilyIndex, VkSurfaceKHR surface) const;
  VkSurfaceCapabilitiesKHR GetCapabilities(VkSurfaceKHR surface) const;
  std::vector<VkSurfaceFormatKHR> GetFormats(VkSurfaceKHR surface) const;
  std::vector<VkPresentModeKHR> GetPresentModes(VkSurfaceKHR surface) const;
  VkFormatProperties GetFormatProperties(const VkFormat format) const;
  VkImageFormatProperties GetSurfaceImageFormatProperties(const VkSurfaceFormatKHR& surfaceFormat) const;

private:
  VkPhysicalDevice physicalDevice;
  VkPhysicalDeviceMemoryProperties memoryProperties;
};

#endif // VULKAN_SRC_VULKAN_PHYSICALDEVICE_H
