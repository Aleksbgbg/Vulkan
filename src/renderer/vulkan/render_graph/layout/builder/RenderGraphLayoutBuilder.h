#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_RENDERGRAPHLAYOUTBUILDER_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_RENDERGRAPHLAYOUTBUILDER_H_

#include <string_view>
#include <unordered_map>

#include "DescriptorSetBuilder.h"
#include "ShaderBuilder.h"
#include "VertexAttributesBuilder.h"
#include "renderer/vulkan/api/structures/GraphicsPipelineCreateInfo.h"
#include "renderer/vulkan/api/structures/VertexInputBindingDescription.h"
#include "renderer/vulkan/render_graph/layout/PipelineKey.h"
#include "renderer/vulkan/render_graph/layout/RenderGraphLayout.h"

class RenderGraphLayoutBuilder {
 public:
  using TemplateKey = u32;

  RenderGraphLayoutBuilder();

  RenderGraphLayoutBuilder& ComputeGlobalDescriptors(
      const DescriptorSetBuilder& builder);
  RenderGraphLayoutBuilder& RenderGlobalDescriptors(
      const DescriptorSetBuilder& builder);

  RenderGraphLayoutBuilder& ComputePipeline(
      PipelineKey key, const DescriptorSetBuilder& descriptorSetBuilder,
      const ShaderBuilder& shaderBuilder);

  RenderGraphLayoutBuilder& GraphicsPipelineTemplate(
      TemplateKey key, GraphicsPipelineCreateInfoBuilder builder);
  RenderGraphLayoutBuilder& RenderPipeline(
      const PipelineKey key, const TemplateKey templateKey,
      const VertexInputBindingDescriptionBuilder& vertexBindingBuilder,
      const VertexAttributesBuilder& vertexAttributesBuilder,
      const DescriptorSetBuilder& descriptorSetBuilder,
      const ShaderBuilder& shaderBuilder);

  RenderGraphLayout Build();

 private:
  std::unordered_map<TemplateKey, GraphicsPipelineCreateInfoBuilder>
      graphicsPipelineTemplates;
  RenderGraphLayout renderGraphLayout_;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_RENDERGRAPHLAYOUTBUILDER_H_
