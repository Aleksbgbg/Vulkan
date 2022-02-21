#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEMULTISAMPLESTATECREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEMULTISAMPLESTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(PipelineMultisampleStateCreateInfoBuilder,
                  VkPipelineMultisampleStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO)
STRUCTURE_SETTER(RasterizationSamples, const VkSampleCountFlagBits,
                 rasterizationSamples)
STRUCTURE_SETTER(SampleShadingEnable, const VkBool32, sampleShadingEnable)
STRUCTURE_SETTER(MinSampleShading, const float, minSampleShading)
STRUCTURE_SETTER(PSampleMask, const VkSampleMask*, pSampleMask)
STRUCTURE_SETTER(AlphaToCoverageEnable, const VkBool32, alphaToCoverageEnable)
STRUCTURE_SETTER(AlphaToOneEnable, const VkBool32, alphaToOneEnable)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEMULTISAMPLESTATECREATEINFO_H_
