#include "DescriptorReferenceBuilder.h"

DescriptorReferenceBuilder::DescriptorReferenceBuilder()
    : descriptorReferences_() {}

DescriptorReferenceBuilder& DescriptorReferenceBuilder::AddGlobalSetBindings(
    const u32 binding) {
  return AddGlobalSetBindings({binding});
}

DescriptorReferenceBuilder& DescriptorReferenceBuilder::AddGlobalSetBindings(
    const std::initializer_list<u32>& bindings) {
  for (const u32 reference : bindings) {
    descriptorReferences_.globalBindings.push_back(reference);
  }
  return *this;
}

DescriptorReferenceBuilder& DescriptorReferenceBuilder::AddLocalSetBindings(
    u32 binding) {
  return AddLocalSetBindings({binding});
}

DescriptorReferenceBuilder& DescriptorReferenceBuilder::AddLocalSetBindings(
    const std::initializer_list<u32>& bindings) {
  for (const u32 reference : bindings) {
    descriptorReferences_.localBindings.push_back(reference);
  }
  return *this;
}

DescriptorReferenceBuilder::DescriptorReferences
DescriptorReferenceBuilder::Build() {
  return std::move(descriptorReferences_);
}

DescriptorReferenceBuilder::DescriptorReferences
DescriptorReferenceBuilder::Build() const {
  return descriptorReferences_;
}
