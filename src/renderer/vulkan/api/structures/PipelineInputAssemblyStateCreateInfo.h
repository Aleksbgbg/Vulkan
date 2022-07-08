#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEINPUTASSEMBLYSTATECREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEINPUTASSEMBLYSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(PipelineInputAssemblyStateCreateInfoBuilder,
                  VkPipelineInputAssemblyStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO)
STRUCTURE_SETTER(Topology, const VkPrimitiveTopology, topology)
STRUCTURE_SETTER(PrimitiveRestartEnable, const VkBool32, primitiveRestartEnable)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEINPUTASSEMBLYSTATECREATEINFO_H_
