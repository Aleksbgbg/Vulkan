#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_VERTEXATTRIBUTESBUILDER_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_VERTEXATTRIBUTESBUILDER_H_

#include <list>
#include <vector>

#include "renderer/vulkan/api/structures/VertexInputAttributeDescription.h"

class VertexAttributesBuilder {
 public:
  VertexAttributesBuilder();

  VertexAttributesBuilder& AddDescription(
      const VertexInputAttributeDescriptionBuilder& builder);

  std::vector<VkVertexInputAttributeDescription> Build() const;

 private:
  std::list<VkVertexInputAttributeDescription> descriptions_;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_VERTEXATTRIBUTESBUILDER_H_
