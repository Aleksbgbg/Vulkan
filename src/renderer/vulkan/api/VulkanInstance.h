#ifndef VULKAN_SRC_RENDERER_VULKAN_API_VULKANINSTANCE_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_VULKANINSTANCE_H_

#include <vector>

#include "DebugUtilsMessenger.h"
#include "PhysicalDevice.h"
#include "Surface.h"
#include "SurfaceFactory.h"
#include "renderer/vulkan/api/structures/DebugUtilsMessengerCreateInfo.h"
#include "renderer/vulkan/api/structures/InstanceCreateInfo.h"
#include "util/types.h"

class VulkanInstance {
 public:
  VulkanInstance();
  explicit VulkanInstance(const InstanceCreateInfoBuilder& infoBuilder);

  VulkanInstance(const VulkanInstance&) = delete;
  VulkanInstance(VulkanInstance&& other) noexcept;

  ~VulkanInstance();

  VulkanInstance& operator=(const VulkanInstance&) = delete;
  VulkanInstance& operator=(VulkanInstance&& other) noexcept;

  DebugUtilsMessenger CreateDebugUtilsMessenger(
      DebugUtilsMessengerCreateInfoExtBuilder& infoBuilder) const;
  Surface CreateSurface(const SurfaceFactory& surfaceFactory) const;

  std::vector<PhysicalDevice> GetPhysicalDevices() const;

  static void LoadInstanceExtensionProperties(
      u32* count, VkExtensionProperties* properties);
  static void LoadInstanceLayerProperties(u32* count,
                                          VkLayerProperties* properties);

 private:
  VkInstance instance;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_VULKANINSTANCE_H_
