#include "DescriptorSet.h"

DescriptorSet::WriteDescriptorSet::WriteDescriptorSet(
    DescriptorBufferInfoBuilder descriptorBuilder,
    WriteDescriptorSetBuilder writeBuilder)
    : type(Type::Buffer),
      info({.bufferInfo = descriptorBuilder.BuildObject()}),
      writeBuilder(writeBuilder) {
  this->writeBuilder.SetPBufferInfo(&info.bufferInfo);
}

DescriptorSet::WriteDescriptorSet::WriteDescriptorSet(
    DescriptorImageInfoBuilder descriptorBuilder,
    WriteDescriptorSetBuilder writeBuilder)
    : type(Type::Image),
      info({.imageInfo = descriptorBuilder.BuildObject()}),
      writeBuilder(writeBuilder) {
  this->writeBuilder.SetPImageInfo(&info.imageInfo);
}

DescriptorSet::WriteDescriptorSet::WriteDescriptorSet(
    const DescriptorSet::WriteDescriptorSet& other) noexcept
    : type(other.type), info(other.info), writeBuilder(other.writeBuilder) {
  ReassignPointers();
}

DescriptorSet::WriteDescriptorSet::WriteDescriptorSet(
    DescriptorSet::WriteDescriptorSet&& other) noexcept
    : type(std::move(other.type)),
      info(std::move(other.info)),
      writeBuilder(std::move(other.writeBuilder)) {
  ReassignPointers();
}

DescriptorSet::WriteDescriptorSet& DescriptorSet::WriteDescriptorSet::operator=(
    const DescriptorSet::WriteDescriptorSet& other) noexcept {
  type = other.type;
  info = other.info;
  writeBuilder = other.writeBuilder;

  ReassignPointers();

  return *this;
}

DescriptorSet::WriteDescriptorSet& DescriptorSet::WriteDescriptorSet::operator=(
    DescriptorSet::WriteDescriptorSet&& other) noexcept {
  type = std::move(other.type);
  info = std::move(other.info);
  writeBuilder = std::move(other.writeBuilder);

  ReassignPointers();

  return *this;
}

VkWriteDescriptorSet DescriptorSet::WriteDescriptorSet::Build() const {
  return writeBuilder;
}

void DescriptorSet::WriteDescriptorSet::ReassignPointers() {
  switch (type) {
    case Type::Buffer:
      writeBuilder.SetPBufferInfo(&info.bufferInfo);
      break;

    case Type::Image:
      writeBuilder.SetPImageInfo(&info.imageInfo);
      break;
  }
}

DescriptorSet::DescriptorSet(VkDescriptorSet descriptorSet)
    : descriptorSet(descriptorSet) {}

DescriptorSet::WriteDescriptorSet DescriptorSet::CreateBufferWrite(
    const Buffer& buffer, const VkDeviceSize range,
    const VkDescriptorType descriptorType, const u32 binding) const {
  return WriteDescriptorSet(DescriptorBufferInfoBuilder()
                                .SetBuffer(buffer.buffer)
                                .SetOffset(0)
                                .SetRange(range),
                            WriteDescriptorSetBuilder()
                                .SetDstSet(descriptorSet)
                                .SetDescriptorType(descriptorType)
                                .SetDstBinding(binding)
                                .SetDescriptorCount(1));
}

DescriptorSet::WriteDescriptorSet DescriptorSet::CreateImageSamplerWrite(
    const ImageView& samplerImageView, const Sampler& sampler,
    const VkImageLayout imageLayout, const u32 binding) const {
  return WriteDescriptorSet(
      DescriptorImageInfoBuilder()
          .SetImageView(samplerImageView.imageView)
          .SetSampler(sampler.sampler)
          .SetImageLayout(imageLayout),
      WriteDescriptorSetBuilder()
          .SetDstSet(descriptorSet)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetDstBinding(binding)
          .SetDescriptorCount(1));
}
