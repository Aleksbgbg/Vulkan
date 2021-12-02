#include "RenderPipeline.h"

#include <array>
#include <vector>

#include "vulkan/structures/Rect2D.h"
#include "vulkan/structures/Viewport.h"
#include "vulkan/structures/default.h"

RenderPipeline::RenderPipeline(
    const Initializer& initializer,
    const ShaderModuleFactory& shaderModuleFactory,
    const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
    const PipelineStateFactory& pipelineStateFactory) {
  constexpr const std::array<VkDynamicState, 2> dynamicStates = {
      VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  const PipelineLayoutCreateInfoBuilder pipelineLayout =
      pipelineStateFactory.CreatePipelineLayout();
  PipelineVertexInputStateCreateInfoBuilder vertexInputState =
      pipelineStateFactory.CreateVertexInputState();

  pipeline = initializer.CreateGraphicsPipeline(
      pipelineStateFactory.LoadShaders(shaderModuleFactory),
      descriptorSetLayouts, pipelineLayout,
      GraphicsPipelineCreateInfoBuilder()
          .SetPDepthStencilState(PipelineDepthStencilStateCreateInfoBuilder()
                                     .SetDepthTestEnable(VK_TRUE)
                                     .SetDepthWriteEnable(VK_TRUE)
                                     .SetDepthCompareOp(VK_COMPARE_OP_LESS)
                                     .SetDepthBoundsTestEnable(VK_FALSE))
          .SetPVertexInputState(vertexInputState)
          .SetPInputAssemblyState(
              PipelineInputAssemblyStateCreateInfoBuilder().SetTopology(
                  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST))
          .SetPViewportState(PipelineViewportStateCreateInfoBuilder()
                                 .SetViewportCount(1)
                                 .SetScissorCount(1))
          .SetPDynamicState(PipelineDynamicStateCreateInfoBuilder()
                                .SetDynamicStateCount(dynamicStates.size())
                                .SetPDynamicStates(dynamicStates.data()))
          .SetPRasterizationState(
              PipelineRasterizationStateCreateInfoBuilder()
                  .SetCullMode(VK_CULL_MODE_BACK_BIT)
                  .SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE)
                  .SetPolygonMode(VK_POLYGON_MODE_FILL)
                  .SetLineWidth(1.0f))
          .SetPMultisampleState(
              initializer.CreateMultisampleState().SetMinSampleShading(1.0f))
          .SetPColorBlendState(
              PipelineColorBlendStateCreateInfoBuilder()
                  .SetAttachmentCount(1)
                  .SetPAttachments(
                      PipelineColorBlendAttachmentStateBuilder()
                          .SetColorWriteMask(VK_COLOR_COMPONENT_R_BIT |
                                             VK_COLOR_COMPONENT_G_BIT |
                                             VK_COLOR_COMPONENT_B_BIT |
                                             VK_COLOR_COMPONENT_A_BIT)
                          .SetSrcColorBlendFactor(VK_BLEND_FACTOR_ONE)
                          .SetDstColorBlendFactor(VK_BLEND_FACTOR_ZERO)
                          .SetColorBlendOp(VK_BLEND_OP_ADD)
                          .SetSrcAlphaBlendFactor(VK_BLEND_FACTOR_ONE)
                          .SetDstAlphaBlendFactor(VK_BLEND_FACTOR_ZERO)
                          .SetAlphaBlendOp(VK_BLEND_OP_ADD))));
}

const PipelineLayout& RenderPipeline::GetLayout() const {
  return pipeline.GetLayout();
}

void RenderPipeline::Bind(const CommandBuffer& commandBuffer,
                          const wnd::Window& window) const {
  const Recti windowRecti = window.GetRect();
  const Rectf windowRectf = windowRecti;

  commandBuffer.CmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  commandBuffer.CmdSetViewport(ViewportBuilder(VIEWPORT_BASE)
                                   .SetWidth(windowRectf.Width())
                                   .SetHeight(windowRectf.Height()));
  commandBuffer.CmdSetScissor(
      Rect2DBuilder()
          .SetOffset(OFFSET2D_ZERO)
          .SetExtent(Extent2DBuilder()
                         .SetWidth(windowRecti.Width())
                         .SetHeight(windowRecti.Height())));
}
