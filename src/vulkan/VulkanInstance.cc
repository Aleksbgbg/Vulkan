#include "VulkanInstance.h"

#include "error.h"
#include "util.h"

VulkanInstance::VulkanInstance() : instance(nullptr) {}

VulkanInstance::VulkanInstance(const InstanceCreateInfoBuilder& infoBuilder) {
  PROCEED_ON_VALID_RESULT(
      vkCreateInstance(infoBuilder.Build(), nullptr, &instance));
}

VulkanInstance::VulkanInstance(VulkanInstance&& other) noexcept
    : instance(other.instance) {
  other.instance = nullptr;
}

VulkanInstance::~VulkanInstance() {
  if (instance != nullptr) {
    vkDestroyInstance(instance, nullptr);
  }
}

VulkanInstance& VulkanInstance::operator=(VulkanInstance&& other) noexcept {
  std::swap(instance, other.instance);
  return *this;
}

DebugUtilsMessenger VulkanInstance::CreateDebugUtilsMessenger(
    DebugUtilsMessengerCreateInfoExtBuilder& infoBuilder) const {
  return DebugUtilsMessenger(instance, infoBuilder);
}

Surface VulkanInstance::CreateSurface(
    const SurfaceFactory& surfaceFactory) const {
  return surfaceFactory.CreateWindowSurface(instance);
}

std::vector<PhysicalDevice> VulkanInstance::GetPhysicalDevices() const {
  std::vector<VkPhysicalDevice> physicalDevices = LoadArray(
      [this](u32* physicalDeviceCount, VkPhysicalDevice* physicalDevices) {
        PROCEED_ON_VALID_RESULT(vkEnumeratePhysicalDevices(
            instance, physicalDeviceCount, physicalDevices))
      });

  return std::vector<PhysicalDevice>(physicalDevices.begin(),
                                     physicalDevices.end());
}

void VulkanInstance::LoadInstanceExtensionProperties(
    u32* const count, VkExtensionProperties* const properties) {
  PROCEED_ON_VALID_RESULT(
      vkEnumerateInstanceExtensionProperties(nullptr, count, properties))
}

void VulkanInstance::LoadInstanceLayerProperties(
    u32* const count, VkLayerProperties* const properties) {
  PROCEED_ON_VALID_RESULT(vkEnumerateInstanceLayerProperties(count, properties))
}
