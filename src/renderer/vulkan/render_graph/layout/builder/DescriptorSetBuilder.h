#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_DESCRIPTORSETBUILDER_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_DESCRIPTORSETBUILDER_H_

#include "renderer/vulkan/render_graph/layout/Binding.h"
#include "renderer/vulkan/render_graph/layout/DescriptorSetStructure.h"

class DescriptorSetBuilder {
 public:
  DescriptorSetBuilder();

  DescriptorSetBuilder& AddBinding(Binding binding);

  DescriptorSetStructure Build();
  DescriptorSetStructure Build() const;

 private:
  u32 bindingIndex_;
  DescriptorSetStructure descriptorSetStructure_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_DESCRIPTORSETBUILDER_H_
