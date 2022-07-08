#ifndef SRC_RENDERER_VULKAN_API_SURFACEFACTORY_H_
#define SRC_RENDERER_VULKAN_API_SURFACEFACTORY_H_

#include <vulkan/vulkan.h>

#include "renderer/vulkan/api/Surface.h"

namespace vk {

class SurfaceFactory {
 public:
  virtual ~SurfaceFactory() = default;

  virtual Surface CreateWindowSurface(VkInstance instance) const = 0;
};

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_SURFACEFACTORY_H_
