#include "ShaderModule.h"

#include "error.h"

ShaderModule::ShaderModule(VkDevice device, const VkShaderStageFlagBits stage,
                           ShaderModuleCreateInfoBuilder& infoBuilder)
    : device(device), stage(stage) {
  PROCEED_ON_VALID_RESULT(vkCreateShaderModule(device, infoBuilder.Build(),
                                               nullptr, &shaderModule));
}

ShaderModule::~ShaderModule() {
  if (shaderModule != nullptr) {
    vkDestroyShaderModule(device, shaderModule, nullptr);
  }
}
