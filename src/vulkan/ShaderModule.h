#ifndef VULKAN_SRC_VULKAN_SHADERMODULE_H
#define VULKAN_SRC_VULKAN_SHADERMODULE_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/ShaderModuleCreateInfo.h"

class ShaderModule {
  friend class Pipeline;
public:
  ShaderModule() = default;
  ShaderModule(VkDevice device, const VkShaderStageFlagBits stage, ShaderModuleCreateInfoBuilder& infoBuilder);

  ShaderModule(const ShaderModule&) = delete;
  ShaderModule(ShaderModule&& other) noexcept
    : device(other.device), shaderModule(other.shaderModule), stage(other.stage) {
    other.shaderModule = nullptr;
  }

  ~ShaderModule();

  ShaderModule& operator=(const ShaderModule&) = delete;
  ShaderModule& operator=(ShaderModule&& other) noexcept {
    device = other.device;
    std::swap(shaderModule, other.shaderModule);
    stage = other.stage;
    return *this;
  }

private:
  VkDevice device;
  VkShaderModule shaderModule = nullptr;
  VkShaderStageFlagBits stage;
};

#endif // VULKAN_SRC_VULKAN_SHADERMODULE_H
