#ifndef SRC_RENDERER_VULKAN_API_SURFACE_H_
#define SRC_RENDERER_VULKAN_API_SURFACE_H_

#include <vulkan/vulkan.h>

#include "PhysicalDevice.h"

namespace vk {

class Surface {
  friend class VirtualDevice;

 public:
  Surface();
  Surface(VkInstance instance, VkSurfaceKHR surface);

  Surface(const Surface&) = delete;
  Surface(Surface&& other) noexcept;

  ~Surface();

  Surface& operator=(const Surface&) = delete;
  Surface& operator=(Surface&& other) noexcept;

  bool IsSupportedByPhysicalDevice(const PhysicalDevice& physicalDevice,
                                   const u32 queueFamilyIndex) const;
  VkSurfaceCapabilitiesKHR GetCapabilities(
      const PhysicalDevice& physicalDevice) const;
  std::vector<VkSurfaceFormatKHR> GetFormats(
      const PhysicalDevice& physicalDevice) const;
  std::vector<VkPresentModeKHR> GetPresentModes(
      const PhysicalDevice& physicalDevice) const;

 private:
  VkInstance instance;
  VkSurfaceKHR surface;
};

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_SURFACE_H_
