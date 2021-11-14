#ifndef VULKAN_SRC_VULKAN_STRUCTURES_SHADERMODULECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_SHADERMODULECREATEINFO_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(ShaderModuleCreateInfoBuilder, VkShaderModuleCreateInfo,
                  VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO)
STRUCTURE_SETTER(CodeSize, const size_t, codeSize)
STRUCTURE_SETTER(PCode, const u32*, pCode)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_SHADERMODULECREATEINFO_H
