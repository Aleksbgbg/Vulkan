#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_GRAPHICSPIPELINESTRUCTURE_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_GRAPHICSPIPELINESTRUCTURE_H_

#include <vulkan/vulkan.h>

#include <list>
#include <vector>

#include "DescriptorSetStructure.h"
#include "PipelineKey.h"
#include "ShaderStructure.h"

struct GraphicsPipelineStructure {
  PipelineKey pipelineKey;
  std::string_view name;
  VkVertexInputBindingDescription vertexBinding;
  std::vector<VkVertexInputAttributeDescription> vertexAttributes;
  DescriptorSetStructure descriptors;
  std::list<ShaderStructure> shaders;
  GraphicsPipelineCreateInfoBuilder graphicsPipelineTemplate;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_GRAPHICSPIPELINESTRUCTURE_H_
