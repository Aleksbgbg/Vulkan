#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_RENDERGRAPHLAYOUT_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_RENDERGRAPHLAYOUT_H_

#include <list>

#include "ComputePipelineStructure.h"
#include "DescriptorSetStructure.h"
#include "GraphicsPipelineStructure.h"

struct RenderGraphLayout {
  DescriptorSetStructure globalComputeDescriptors;
  std::list<ComputePipelineStructure> computePipelines;

  DescriptorSetStructure globalGraphicsDescriptors;
  std::list<GraphicsPipelineStructure> graphicsPipelines;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_RENDERGRAPHLAYOUT_H_
