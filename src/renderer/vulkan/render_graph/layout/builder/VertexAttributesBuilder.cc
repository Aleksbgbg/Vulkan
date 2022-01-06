#include "VertexAttributesBuilder.h"

VertexAttributesBuilder::VertexAttributesBuilder() : descriptions_() {}

VertexAttributesBuilder& VertexAttributesBuilder::AddDescription(
    const VertexInputAttributeDescriptionBuilder& builder) {
  descriptions_.push_back(builder.BuildObject());
  return *this;
}

std::vector<VkVertexInputAttributeDescription> VertexAttributesBuilder::Build()
    const {
  return std::vector<VkVertexInputAttributeDescription>(descriptions_.begin(),
                                                        descriptions_.end());
}
