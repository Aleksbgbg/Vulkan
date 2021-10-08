#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEMULTISAMPLESTATECREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEMULTISAMPLESTATECREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(PipelineMultisampleStateCreateInfoBuilder,
                  VkPipelineMultisampleStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO)
STRUCTURE_SETTER(PipelineMultisampleStateCreateInfoBuilder,
                 RasterizationSamples, const VkSampleCountFlagBits,
                 rasterizationSamples)
STRUCTURE_SETTER(PipelineMultisampleStateCreateInfoBuilder, SampleShadingEnable,
                 const VkBool32, sampleShadingEnable)
STRUCTURE_SETTER(PipelineMultisampleStateCreateInfoBuilder, MinSampleShading,
                 const float, minSampleShading)
STRUCTURE_SETTER(PipelineMultisampleStateCreateInfoBuilder, PSampleMask,
                 const VkSampleMask*, pSampleMask)
STRUCTURE_SETTER(PipelineMultisampleStateCreateInfoBuilder,
                 AlphaToCoverageEnable, const VkBool32, alphaToCoverageEnable)
STRUCTURE_SETTER(PipelineMultisampleStateCreateInfoBuilder, AlphaToOneEnable,
                 const VkBool32, alphaToOneEnable)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEMULTISAMPLESTATECREATEINFO_H
