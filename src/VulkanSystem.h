#ifndef VULKAN_SRC_VULKANSYSTEM_H_
#define VULKAN_SRC_VULKANSYSTEM_H_

class VulkanSystem {
 public:
  virtual ~VulkanSystem() = default;

  virtual const char* SurfaceExtensionName() const = 0;
};

#endif  // VULKAN_SRC_VULKANSYSTEM_H_
