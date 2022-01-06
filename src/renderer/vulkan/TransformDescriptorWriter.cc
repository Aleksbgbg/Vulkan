#include "TransformDescriptorWriter.h"

TransformDescriptorWriter::TransformDescriptorWriter(
    const Transformable& transformable)
    : transformable_(transformable) {}

void TransformDescriptorWriter::Fill(
    const std::unordered_map<u32, Descriptor>& descriptors) const {
  const glm::mat4 transform = transformable_.GetTransform();
  descriptors.at(0).buffer.MapCopy(&transform);
}
