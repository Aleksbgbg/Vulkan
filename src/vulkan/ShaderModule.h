#ifndef VULKAN_SRC_VULKAN_SHADERMODULE_H
#define VULKAN_SRC_VULKAN_SHADERMODULE_H

#include <vulkan/vulkan.h>

#include "structures/ShaderModuleCreateInfo.h"

class ShaderModule {
  friend class Pipeline;

 public:
  ShaderModule();
  ShaderModule(VkDevice device, const VkShaderStageFlagBits stage,
               const ShaderModuleCreateInfoBuilder& infoBuilder);

  ShaderModule(const ShaderModule&) = delete;
  ShaderModule(ShaderModule&& other) noexcept;

  ~ShaderModule();

  ShaderModule& operator=(const ShaderModule&) = delete;
  ShaderModule& operator=(ShaderModule&& other) noexcept;

 private:
  VkDevice device;
  VkShaderModule shaderModule;
  VkShaderStageFlagBits stage;
};

#endif  // VULKAN_SRC_VULKAN_SHADERMODULE_H
