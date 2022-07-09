#include "RenderGraphLayoutBuilder.h"

RenderGraphLayoutBuilder::RenderGraphLayoutBuilder() : renderGraphLayout_() {}

RenderGraphLayoutBuilder& RenderGraphLayoutBuilder::ComputeGlobalDescriptors(
    const DescriptorSetBuilder& builder) {
  renderGraphLayout_.globalComputeDescriptors = builder.Build();
  return *this;
}

RenderGraphLayoutBuilder& RenderGraphLayoutBuilder::RenderGlobalDescriptors(
    const DescriptorSetBuilder& builder) {
  renderGraphLayout_.globalGraphicsDescriptors = builder.Build();
  return *this;
}

RenderGraphLayoutBuilder& RenderGraphLayoutBuilder::ComputePipeline(
    PipelineKey key, const DescriptorSetBuilder& descriptorSetBuilder,
    const ShaderBuilder& shaderBuilder) {
  const ShaderBuilder::ShaderBuildResult result = shaderBuilder.Build();

  ComputePipelineStructure pipelineStructure{
      .pipelineKey = key,
      .descriptors = descriptorSetBuilder.Build(),
      .shader = result.shader};
  for (const u32 reference : pipelineStructure.shader.localBindings) {
    pipelineStructure.descriptors.bindings[reference].stageFlags |=
        pipelineStructure.shader.shaderStage;
  }
  renderGraphLayout_.computePipelines.push_back(std::move(pipelineStructure));

  return *this;
}

RenderGraphLayoutBuilder& RenderGraphLayoutBuilder::GraphicsPipelineTemplate(
    const TemplateKey key, const GraphicsPipelineCreateInfoBuilder builder) {
  graphicsPipelineTemplates.insert(std::make_pair(key, builder));
  return *this;
}

RenderGraphLayoutBuilder& RenderGraphLayoutBuilder::RenderPipeline(
    const PipelineKey key, const TemplateKey templateKey,
    const VertexInputBindingDescriptionBuilder& vertexBindingBuilder,
    const VertexAttributesBuilder& vertexAttributesBuilder,
    const DescriptorSetBuilder& descriptorSetBuilder,
    const ShaderBuilder& shaderBuilder) {
  const ShaderBuilder::ShaderBuildResult result = shaderBuilder.Build();

  GraphicsPipelineStructure pipelineStructure{
      .pipelineKey = key,
      .vertexBinding = vertexBindingBuilder.BuildObject(),
      .vertexAttributes = vertexAttributesBuilder.Build(),
      .descriptors = descriptorSetBuilder.Build(),
      .shaders = result.shaders,
      .graphicsPipelineTemplate = graphicsPipelineTemplates.at(templateKey)};
  for (const ShaderStructure& shader : pipelineStructure.shaders) {
    for (const u32 reference : shader.localBindings) {
      pipelineStructure.descriptors.bindings[reference].stageFlags |=
          shader.shaderStage;
    }
  }
  renderGraphLayout_.graphicsPipelines.push_back(std::move(pipelineStructure));

  return *this;
}

RenderGraphLayout RenderGraphLayoutBuilder::Build() {
  for (const ComputePipelineStructure& computePipelineStructure :
       renderGraphLayout_.computePipelines) {
    for (const u32 reference : computePipelineStructure.shader.globalBindings) {
      renderGraphLayout_.globalComputeDescriptors.bindings[reference]
          .stageFlags |= computePipelineStructure.shader.shaderStage;
    }
  }

  for (const GraphicsPipelineStructure& graphicsPipelineStructure :
       renderGraphLayout_.graphicsPipelines) {
    for (const ShaderStructure& shader : graphicsPipelineStructure.shaders) {
      for (const u32 reference : shader.globalBindings) {
        renderGraphLayout_.globalGraphicsDescriptors.bindings[reference]
            .stageFlags |= shader.shaderStage;
      }
    }
  }

  return renderGraphLayout_;
}
