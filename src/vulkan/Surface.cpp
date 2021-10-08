#include "Surface.h"

#include "error.h"

Surface::Surface(VkInstance instance,
                 Win32SurfaceCreateInfoBuilder& infoBuilder)
    : instance(instance) {
  PROCEED_ON_VALID_RESULT(vkCreateWin32SurfaceKHR(instance, infoBuilder.Build(),
                                                  nullptr, &surface));
}

Surface::~Surface() {
  if (surface != nullptr) {
    vkDestroySurfaceKHR(instance, surface, nullptr);
  }
}

bool Surface::IsSupportedByPhysicalDevice(const PhysicalDevice& physicalDevice,
                                          const u32 queueFamilyIndex) const {
  return physicalDevice.SupportsSurface(queueFamilyIndex, surface);
}

VkSurfaceCapabilitiesKHR Surface::GetCapabilities(
    const PhysicalDevice& physicalDevice) const {
  return physicalDevice.GetCapabilities(surface);
}

std::vector<VkSurfaceFormatKHR> Surface::GetFormats(
    const PhysicalDevice& physicalDevice) const {
  return physicalDevice.GetFormats(surface);
}

std::vector<VkPresentModeKHR> Surface::GetPresentModes(
    const PhysicalDevice& physicalDevice) const {
  return physicalDevice.GetPresentModes(surface);
}
