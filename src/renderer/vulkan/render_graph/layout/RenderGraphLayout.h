#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_RENDERGRAPHLAYOUT_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_RENDERGRAPHLAYOUT_H_

#include "ComputePipelineStructure.h"
#include "DescriptorSetStructure.h"
#include "GraphicsPipelineStructure.h"

struct RenderGraphLayout {
  DescriptorSetStructure globalComputeDescriptors;
  std::list<ComputePipelineStructure> computePipelines;

  DescriptorSetStructure globalGraphicsDescriptors;
  std::list<GraphicsPipelineStructure> graphicsPipelines;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_RENDERGRAPHLAYOUT_H_
