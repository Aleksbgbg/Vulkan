#ifndef SRC_RENDERER_VULKAN_VULKANSYSTEM_H_
#define SRC_RENDERER_VULKAN_VULKANSYSTEM_H_

class VulkanSystem {
 public:
  virtual ~VulkanSystem() = default;

  virtual const char* SurfaceExtensionName() const = 0;
};

#endif  // SRC_RENDERER_VULKAN_VULKANSYSTEM_H_
