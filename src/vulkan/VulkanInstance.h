#ifndef VULKAN_SRC_VULKAN_VULKANINSTANCE_H
#define VULKAN_SRC_VULKAN_VULKANINSTANCE_H

#include <vector>

#include "lifetime_semantics.h"
#include "types.h"
#include "structures/InstanceCreateInfo.h"
#include "structures/DebugUtilsMessengerCreateInfo.h"
#include "structures/Win32SurfaceCreateInfo.h"
#include "DebugUtilsMessenger.h"
#include "Surface.h"
#include "PhysicalDevice.h"

class VulkanInstance {
  friend class ImGuiInstance;
public:
  VULKAN_OBJECT_MOVABLE(VulkanInstance, instance)

  VulkanInstance() = default;
  explicit VulkanInstance(InstanceCreateInfoBuilder& infoBuilder);

  VulkanInstance(const VulkanInstance&) = delete;

  ~VulkanInstance();

  VulkanInstance& operator=(const VulkanInstance&) = delete;

  DebugUtilsMessenger CreateDebugUtilsMessenger(DebugUtilsMessengerCreateInfoExtBuilder& infoBuilder) const;
  Surface CreateSurface(Win32SurfaceCreateInfoBuilder& infoBuilder) const;

  std::vector<PhysicalDevice> GetPhysicalDevices() const;

  static void LoadInstanceExtensionProperties(u32* count, VkExtensionProperties* properties);
  static void LoadInstanceLayerProperties(u32* count, VkLayerProperties* properties);

private:
  VkInstance instance = nullptr;
};

#endif // VULKAN_SRC_VULKAN_VULKANINSTANCE_H
