#include "DescriptorSet.h"

// DescriptorSet::WriteDescriptorSet::WriteDescriptorSet() : type(Type::None) {}
//
// DescriptorSet::WriteDescriptorSet::WriteDescriptorSet(
//     const DescriptorSet::WriteDescriptorSet& other) noexcept
//     : writeBuilder(other.writeBuilder), info(other.info) {
//   if (other.type == Type::Image) {
//     writeBuilder.SetPImageInfo(&info.imageInfo);
//   } else {
//     writeBuilder.SetPBufferInfo(&info.bufferInfo);
//   }
// }
//
// DescriptorSet::WriteDescriptorSet::WriteDescriptorSet(
//     DescriptorSet::WriteDescriptorSet&& other) noexcept
//     : writeBuilder(other.writeBuilder), info(other.info) {
//   if (other.type == Type::Image) {
//     writeBuilder.SetPImageInfo(&info.imageInfo);
//   } else {
//     writeBuilder.SetPBufferInfo(&info.bufferInfo);
//   }
// }
//
// DescriptorSet::WriteDescriptorSet&
// DescriptorSet::WriteDescriptorSet::operator=(
//     const DescriptorSet::WriteDescriptorSet& other) noexcept {
//   writeBuilder = other.writeBuilder;
//   info = other.info;
//
//   if (other.type == Type::Image) {
//     writeBuilder.SetPImageInfo(&info.imageInfo);
//   } else {
//     writeBuilder.SetPBufferInfo(&info.bufferInfo);
//   }
//
//   return *this;
// }
//
// DescriptorSet::WriteDescriptorSet&
// DescriptorSet::WriteDescriptorSet::operator=(
//     DescriptorSet::WriteDescriptorSet&& other) noexcept {
//   writeBuilder = other.writeBuilder;
//   info = other.info;
//
//   if (other.type == Type::Image) {
//     writeBuilder.SetPImageInfo(&info.imageInfo);
//   } else {
//     writeBuilder.SetPBufferInfo(&info.bufferInfo);
//   }
//
//   return *this;
// }

DescriptorSet::WriteDescriptorSet::operator VkWriteDescriptorSet() const {
  return writeBuilder;
}

DescriptorSet::DescriptorSet(VkDescriptorSet descriptorSet)
    : descriptorSet(descriptorSet) {}

void DescriptorSet::CreateBufferWrite(
    const Buffer& buffer, const VkDeviceSize range,
    const VkDescriptorType descriptorType,
    DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const {
  CreateBufferWrite(buffer, range, descriptorType, 0, writeDescriptorSet);
}

void DescriptorSet::CreateBufferWrite(
    const Buffer& buffer, const VkDeviceSize range,
    const VkDescriptorType descriptorType, const u32 binding,
    DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const {
  //  writeDescriptorSet.type = WriteDescriptorSet::Type::Buffer;
  writeDescriptorSet.info.bufferInfo = DescriptorBufferInfoBuilder()
                                           .SetBuffer(buffer.buffer)
                                           .SetOffset(0)
                                           .SetRange(range);
  writeDescriptorSet.writeBuilder =
      WriteDescriptorSetBuilder()
          .SetDstSet(descriptorSet)
          .SetDescriptorType(descriptorType)
          .SetDstBinding(binding)
          .SetDescriptorCount(1)
          .SetPBufferInfo(&writeDescriptorSet.info.bufferInfo);
}

void DescriptorSet::CreateImageSamplerWrite(
    const ImageView& samplerImageView, const Sampler& sampler,
    const VkImageLayout imageLayout,
    DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const {
  CreateImageSamplerWrite(samplerImageView, sampler, imageLayout, 0,
                          writeDescriptorSet);
}

void DescriptorSet::CreateImageSamplerWrite(
    const ImageView& samplerImageView, const Sampler& sampler,
    const VkImageLayout imageLayout, const u32 binding,
    DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const {
  //  writeDescriptorSet.type = WriteDescriptorSet::Type::Image;
  writeDescriptorSet.info.imageInfo =
      DescriptorImageInfoBuilder()
          .SetImageView(samplerImageView.imageView)
          .SetSampler(sampler.sampler)
          .SetImageLayout(imageLayout);
  writeDescriptorSet.writeBuilder =
      WriteDescriptorSetBuilder()
          .SetDstSet(descriptorSet)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetDstBinding(binding)
          .SetDescriptorCount(1)
          .SetPImageInfo(&writeDescriptorSet.info.imageInfo);
}
