#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEVERTEXINPUTSTATECREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEVERTEXINPUTSTATECREATEINFO_H_

#include <vulkan/vulkan.h>

#include "VertexInputAttributeDescription.h"
#include "VertexInputBindingDescription.h"
#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(PipelineVertexInputStateCreateInfoBuilder,
                  VkPipelineVertexInputStateCreateInfo,
                  VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO)
STRUCTURE_SETTER(VertexBindingDescriptionCount, const u32,
                 vertexBindingDescriptionCount)
STRUCTURE_SETTER(PVertexBindingDescriptions,
                 const VkVertexInputBindingDescription*,
                 pVertexBindingDescriptions)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PVertexBindingDescriptions,
                                      VertexInputBindingDescriptionBuilder,
                                      pVertexBindingDescriptions)
STRUCTURE_SETTER(VertexAttributeDescriptionCount, const u32,
                 vertexAttributeDescriptionCount)
STRUCTURE_SETTER(PVertexAttributeDescriptions,
                 const VkVertexInputAttributeDescription*,
                 pVertexAttributeDescriptions)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PVertexAttributeDescriptions,
                                      VertexInputAttributeDescriptionBuilder,
                                      pVertexAttributeDescriptions)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_PIPELINEVERTEXINPUTSTATECREATEINFO_H_
