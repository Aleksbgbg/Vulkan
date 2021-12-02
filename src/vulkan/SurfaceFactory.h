#ifndef VULKAN_SRC_VULKAN_SURFACEFACTORY_H_
#define VULKAN_SRC_VULKAN_SURFACEFACTORY_H_

#include <vulkan/vulkan.h>

#include "vulkan/Surface.h"

class SurfaceFactory {
 public:
  virtual ~SurfaceFactory() = default;

  virtual Surface CreateWindowSurface(VkInstance instance) const = 0;
};

#endif  // VULKAN_SRC_VULKAN_SURFACEFACTORY_H_
