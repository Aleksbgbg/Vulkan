#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_RENDERGRAPHLAYOUTBUILDER_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_RENDERGRAPHLAYOUTBUILDER_H_

#include <string_view>

#include "DescriptorSetBuilder.h"
#include "ShaderBuilder.h"
#include "VertexAttributesBuilder.h"
#include "renderer/vulkan/api/structures/VertexInputBindingDescription.h"
#include "renderer/vulkan/render_graph/layout/PipelineKey.h"
#include "renderer/vulkan/render_graph/layout/RenderGraphLayout.h"

class RenderGraphLayoutBuilder {
 public:
  RenderGraphLayoutBuilder();

  RenderGraphLayoutBuilder& ComputeGlobalDescriptors(
      const DescriptorSetBuilder& builder);
  RenderGraphLayoutBuilder& RenderGlobalDescriptors(
      const DescriptorSetBuilder& builder);

  RenderGraphLayoutBuilder& ComputePipeline(
      PipelineKey key, const std::string_view name,
      const DescriptorSetBuilder& descriptorSetBuilder,
      const ShaderBuilder& shaderBuilder);

  RenderGraphLayoutBuilder& RenderPipeline(
      PipelineKey key, const std::string_view name,
      const VertexInputBindingDescriptionBuilder& vertexBindingBuilder,
      const VertexAttributesBuilder& vertexAttributesBuilder,
      const DescriptorSetBuilder& descriptorSetBuilder,
      const ShaderBuilder& shaderBuilder);

  RenderGraphLayout Build();

 private:
  RenderGraphLayout renderGraphLayout_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_RENDERGRAPHLAYOUTBUILDER_H_
