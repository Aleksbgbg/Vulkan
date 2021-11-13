#ifndef VULKAN_SRC_GENERAL_WINDOWING_VULKANWINDOW_H
#define VULKAN_SRC_GENERAL_WINDOWING_VULKANWINDOW_H

#include <vulkan/vulkan.h>

#include "vulkan/Surface.h"

class VulkanWindow {
 public:
  virtual ~VulkanWindow() = default;

  virtual Surface CreateWindowSurface(VkInstance instance) const = 0;
};

#endif  // VULKAN_SRC_GENERAL_WINDOWING_VULKANWINDOW_H
