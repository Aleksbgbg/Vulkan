#include "DescriptorSetBuilder.h"

DescriptorSetBuilder::DescriptorSetBuilder()
    : bindingIndex_(0), descriptorSetStructure_() {}

DescriptorSetBuilder& DescriptorSetBuilder::AddBinding(Binding binding) {
  binding.index = bindingIndex_++;
  descriptorSetStructure_.bindings.push_back(binding);
  return *this;
}

DescriptorSetStructure DescriptorSetBuilder::Build() {
  return std::move(descriptorSetStructure_);
}

DescriptorSetStructure DescriptorSetBuilder::Build() const {
  return descriptorSetStructure_;
}
