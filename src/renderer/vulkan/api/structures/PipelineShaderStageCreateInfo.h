#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINESHADERSTAGECREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINESHADERSTAGECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(PipelineShaderStageCreateInfoBuilder,
                  VkPipelineShaderStageCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO)
STRUCTURE_SETTER(Flags, const VkPipelineShaderStageCreateFlags, flags)
STRUCTURE_SETTER(Stage, const VkShaderStageFlagBits, stage)
STRUCTURE_SETTER(Module, VkShaderModule, module)
STRUCTURE_SETTER(PName, const char*, pName)
STRUCTURE_SETTER(PSpecializationInfo, const VkSpecializationInfo*,
                 pSpecializationInfo)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINESHADERSTAGECREATEINFO_H_
