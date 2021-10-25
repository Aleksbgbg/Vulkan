#include "DescriptorSet.h"

DescriptorSet::DescriptorSet(VkDescriptorSet descriptorSet)
    : descriptorSet(descriptorSet) {}

void DescriptorSet::CreateBufferWrite(
    const Buffer& buffer, const VkDeviceSize range,
    const VkDescriptorType descriptorType,
    DescriptorSet::WriteDescriptorSet& writeDescriptorSet) const {
  writeDescriptorSet.info.bufferInfo = DescriptorBufferInfoBuilder()
                                           .SetBuffer(buffer.buffer)
                                           .SetOffset(0)
                                           .SetRange(range);
  writeDescriptorSet.writeBuilder =
      WriteDescriptorSetBuilder()
          .SetDstSet(descriptorSet)
          .SetDescriptorType(descriptorType)
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
