#include "ShaderModule.h"

#include <utility>

#include "error.h"

namespace vk {

ShaderModule::ShaderModule() : shaderModule(nullptr) {}

ShaderModule::ShaderModule(VkDevice device, const VkShaderStageFlagBits stage,
                           const ShaderModuleCreateInfoBuilder& infoBuilder)
    : device(device), stage(stage) {
  PROCEED_ON_VALID_RESULT(vkCreateShaderModule(device, infoBuilder.Build(),
                                               nullptr, &shaderModule));
}

ShaderModule::ShaderModule(ShaderModule&& other) noexcept
    : device(other.device),
      shaderModule(other.shaderModule),
      stage(other.stage) {
  other.shaderModule = nullptr;
}

ShaderModule::~ShaderModule() {
  if (shaderModule != nullptr) {
    vkDestroyShaderModule(device, shaderModule, nullptr);
  }
}

ShaderModule& ShaderModule::operator=(ShaderModule&& other) noexcept {
  this->~ShaderModule();
  new (this) ShaderModule(std::move(other));
  return *this;
}

}  // namespace vk
