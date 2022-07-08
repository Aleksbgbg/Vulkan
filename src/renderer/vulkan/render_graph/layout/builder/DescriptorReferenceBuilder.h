#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_DESCRIPTORREFERENCEBUILDER_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_DESCRIPTORREFERENCEBUILDER_H_

#include <initializer_list>
#include <list>

#include "core/types.h"

class DescriptorReferenceBuilder {
 public:
  DescriptorReferenceBuilder();

  DescriptorReferenceBuilder& AddGlobalSetBindings(u32 binding);
  DescriptorReferenceBuilder& AddGlobalSetBindings(
      const std::initializer_list<u32>& bindings);

  DescriptorReferenceBuilder& AddLocalSetBindings(u32 binding);
  DescriptorReferenceBuilder& AddLocalSetBindings(
      const std::initializer_list<u32>& bindings);

  struct DescriptorReferences {
    std::list<u32> globalBindings;
    std::list<u32> localBindings;
  };

  DescriptorReferences Build();
  DescriptorReferences Build() const;

 private:
  DescriptorReferences descriptorReferences_;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_DESCRIPTORREFERENCEBUILDER_H_
