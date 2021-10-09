#include "DescriptorSet.h"

DescriptorSet::DescriptorSet(VkDescriptorSet descriptorSet)
    : descriptorSet(descriptorSet) {}

void DescriptorSet::CreateBufferWrite(
    const Buffer& buffer,
    DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const {
  writeDescriptorSet.info.bufferInfo = DescriptorBufferInfoBuilder()
                                           .SetBuffer(buffer.buffer)
                                           .SetOffset(0)
                                           .SetRange(buffer.Size());
  writeDescriptorSet.writeBuilder =
      WriteDescriptorSetBuilder()
          .SetDstSet(descriptorSet)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetDescriptorCount(1)
          .SetPBufferInfo(&writeDescriptorSet.info.bufferInfo);
}

void DescriptorSet::CreateImageSamplerWrite(
    const ImageView& samplerImageView, const Sampler& sampler,
    const VkImageLayout imageLayout,
    DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const {
  writeDescriptorSet.info.imageInfo =
      DescriptorImageInfoBuilder()
          .SetImageView(samplerImageView.imageView)
          .SetSampler(sampler.sampler)
          .SetImageLayout(imageLayout);
  writeDescriptorSet.writeBuilder =
      WriteDescriptorSetBuilder()
          .SetDstSet(descriptorSet)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetDescriptorCount(1)
          .SetPImageInfo(&writeDescriptorSet.info.imageInfo);
}