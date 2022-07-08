#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_COMPUTEPIPELINESTRUCTURE_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_COMPUTEPIPELINESTRUCTURE_H_

#include "DescriptorSetStructure.h"
#include "PipelineKey.h"
#include "ShaderStructure.h"

struct ComputePipelineStructure {
  PipelineKey pipelineKey;
  std::string_view name;
  DescriptorSetStructure descriptors;
  ShaderStructure shader;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_COMPUTEPIPELINESTRUCTURE_H_
