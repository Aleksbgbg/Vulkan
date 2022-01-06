#include "InsertPipelineBuilder.h"

PipelineDescriptorReference::PipelineDescriptorReference(const PipelineKey key,
                                                         const u32 descriptorId)
    : key(key), descriptorId(descriptorId) {}

bool PipelineDescriptorReference::operator==(
    const PipelineDescriptorReference& other) const {
  return (key == other.key) && (descriptorId == other.descriptorId);
}

std::size_t std::hash<PipelineDescriptorReference>::operator()(
    const PipelineDescriptorReference& value) const {
  return std::hash<PipelineKey>()(value.key) ^
         std::hash<u32>()(value.descriptorId);
}

InsertPipelineBuilder::InsertPipelineBuilder()
    : currentConnectionId_(0), result_() {}

InsertPipelineBuilder&& InsertPipelineBuilder::Compute(
    const PipelineKey key, InsertComputeInfo computeInfo) {
  result_.computes.push_back(
      {.key = key, .insertInfo = std::move(computeInfo)});
  return std::move(*this);
}

InsertPipelineBuilder&& InsertPipelineBuilder::Render(
    const PipelineKey key, InsertRenderInfo renderInfo) {
  result_.renders.push_back({.key = key, .insertInfo = std::move(renderInfo)});
  return std::move(*this);
}

InsertPipelineBuilder&& InsertPipelineBuilder::ConnectDescriptors(
    const std::initializer_list<PipelineDescriptorReference>
        descriptorReferences) {
  const BuildResult::ConnectionId connectionId = ++currentConnectionId_;
  for (const PipelineDescriptorReference& reference : descriptorReferences) {
    result_.pipelineConnections.insert(std::make_pair(reference, connectionId));
  }

  return std::move(*this);
}

InsertPipelineBuilder::BuildResult& InsertPipelineBuilder::Build() {
  return result_;
}
