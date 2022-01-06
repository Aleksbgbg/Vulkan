#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_COMPUTEPIPELINESTRUCTURE_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_COMPUTEPIPELINESTRUCTURE_H_

#include "DescriptorSetStructure.h"
#include "PipelineKey.h"
#include "ShaderStructure.h"

struct ComputePipelineStructure {
  PipelineKey pipelineKey;
  std::string_view name;
  DescriptorSetStructure descriptors;
  ShaderStructure shader;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_COMPUTEPIPELINESTRUCTURE_H_
