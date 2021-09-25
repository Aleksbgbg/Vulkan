#include <vulkan/structures/WriteDescriptorSet.h>
#include "DescriptorSet.h"

DescriptorSet::DescriptorSet(VkDescriptorSet descriptorSet) : descriptorSet(descriptorSet) {

}

DescriptorSet::WriteDescriptorSetBuild DescriptorSet::CreateBufferWrite(const Buffer& buffer) const {
  DescriptorBufferInfoBuilder bufferInfoBuilder =
      DescriptorBufferInfoBuilder()
          .SetBuffer(buffer.buffer)
          .SetOffset(0)
          .SetRange(buffer.Size());

  return {
      .builder =
          WriteDescriptorSetBuilder()
              .SetDstSet(descriptorSet)
              .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
              .SetDescriptorCount(1),
      .info = {
          .bufferInfo = bufferInfoBuilder.BuildObject()
      }
  };
}

DescriptorSet::WriteDescriptorSetBuild DescriptorSet::CreateImageSamplerWrite(
    const ImageView& samplerImageView, const Sampler& sampler, const VkImageLayout imageLayout) const {
  DescriptorImageInfoBuilder imageInfoBuilder =
      DescriptorImageInfoBuilder()
          .SetImageView(samplerImageView.imageView)
          .SetSampler(sampler.sampler)
          .SetImageLayout(imageLayout);

  return {
    .builder =
        WriteDescriptorSetBuilder()
            .SetDstSet(descriptorSet)
            .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            .SetDescriptorCount(1),
    .info = {
        .imageInfo = imageInfoBuilder.BuildObject()
    }
  };
}