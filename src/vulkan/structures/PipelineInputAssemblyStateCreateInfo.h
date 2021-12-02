#ifndef VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEINPUTASSEMBLYSTATECREATEINFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEINPUTASSEMBLYSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(PipelineInputAssemblyStateCreateInfoBuilder,
                  VkPipelineInputAssemblyStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO)
STRUCTURE_SETTER(Topology, const VkPrimitiveTopology, topology)
STRUCTURE_SETTER(PrimitiveRestartEnable, const VkBool32, primitiveRestartEnable)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_PIPELINEINPUTASSEMBLYSTATECREATEINFO_H_
