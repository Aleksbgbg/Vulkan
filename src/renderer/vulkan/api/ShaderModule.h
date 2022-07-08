#ifndef SRC_RENDERER_VULKAN_API_SHADERMODULE_H_
#define SRC_RENDERER_VULKAN_API_SHADERMODULE_H_

#include <vulkan/vulkan.h>

#include "renderer/vulkan/api/structures/ShaderModuleCreateInfo.h"

namespace vk {

class ShaderModule {
  friend class GraphicsPipeline;
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

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_SHADERMODULE_H_
