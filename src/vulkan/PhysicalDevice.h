#ifndef VULKAN_SRC_VULKAN_PHYSICALDEVICE_H_
#define VULKAN_SRC_VULKAN_PHYSICALDEVICE_H_

#include <vulkan/vulkan.h>

#include <functional>
#include <optional>

#include "Queue.h"
#include "VirtualDevice.h"
#include "structures/DeviceCreateInfo.h"

class PhysicalDevice {
  friend class ImGuiInstance;

 public:
  PhysicalDevice() = default;
  explicit PhysicalDevice(VkPhysicalDevice physicalDevice);

  VkPhysicalDeviceProperties GetProperties() const;
  const VkPhysicalDeviceMemoryProperties* GetMemoryProperties() const;
  std::optional<u32> FindAppropriateQueueFamily(
      const VkQueueFlags requiredProperties,
      const std::function<bool(u32)>& predicate) const;
  VirtualDevice CreateVirtualDevice(
      const DeviceCreateInfoBuilder& infoBuilder) const;
  bool SupportsSurface(const u32 queueFamilyIndex, VkSurfaceKHR surface) const;
  VkSurfaceCapabilitiesKHR GetCapabilities(VkSurfaceKHR surface) const;
  std::vector<VkSurfaceFormatKHR> GetFormats(VkSurfaceKHR surface) const;
  std::vector<VkPresentModeKHR> GetPresentModes(VkSurfaceKHR surface) const;
  VkFormatProperties GetFormatProperties(const VkFormat format) const;

 private:
  VkPhysicalDevice physicalDevice;
  VkPhysicalDeviceMemoryProperties memoryProperties;
};

#endif  // VULKAN_SRC_VULKAN_PHYSICALDEVICE_H_
