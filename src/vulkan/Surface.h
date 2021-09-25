#ifndef VULKAN_SRC_VULKAN_SURFACE_H
#define VULKAN_SRC_VULKAN_SURFACE_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "vulkan/structures/Win32SurfaceCreateInfo.h"
#include "PhysicalDevice.h"

class Surface {
  friend class VirtualDevice;
public:
  VULKAN_OBJECT_MOVABLE_ROOT(Surface, instance, surface)

  Surface() = default;
  explicit Surface(VkInstance instance, Win32SurfaceCreateInfoBuilder& infoBuilder);

  Surface(const Surface&) = delete;

  ~Surface();

  Surface& operator=(const Surface&) = delete;

  bool IsSupportedByPhysicalDevice(const PhysicalDevice& physicalDevice, const u32 queueFamilyIndex) const;
  VkSurfaceCapabilitiesKHR GetCapabilities(const PhysicalDevice& physicalDevice) const;
  std::vector<VkSurfaceFormatKHR> GetFormats(const PhysicalDevice& physicalDevice) const;
  std::vector<VkPresentModeKHR> GetPresentModes(const PhysicalDevice& physicalDevice) const;

private:
  VkInstance instance;
  VkSurfaceKHR surface = nullptr;
};

#endif // VULKAN_SRC_VULKAN_SURFACE_H
