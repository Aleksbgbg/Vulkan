#ifndef VULKAN_SRC_RENDERER_VULKAN_API_SHADERMODULE_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_SHADERMODULE_H_

#include <vulkan/vulkan.h>

#include "renderer/vulkan/api/structures/ShaderModuleCreateInfo.h"

class ShaderModule {
  friend class Pipeline;
  friend class ComputePipeline;

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

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_SHADERMODULE_H_
