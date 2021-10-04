#include <vulkan/structures/WriteDescriptorSet.h>
#include "DescriptorSet.h"

DescriptorSet::DescriptorSet(VkDescriptorSet descriptorSet) : descriptorSet(descriptorSet) {

}

std::unique_ptr<DescriptorSet::WriteDescriptorSetBuild> DescriptorSet::CreateBufferWrite(const Buffer& buffer) const {
  std::unique_ptr<WriteDescriptorSetBuild> writeDescriptorSet = std::make_unique<WriteDescriptorSetBuild>();
  writeDescriptorSet->info.bufferInfo =
      DescriptorBufferInfoBuilder()
          .SetBuffer(buffer.buffer)
          .SetOffset(0)
          .SetRange(buffer.Size())
          .BuildObject();
  writeDescriptorSet->writeBuilder =
      WriteDescriptorSetBuilder()
          .SetDstSet(descriptorSet)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetDescriptorCount(1)
          .SetPBufferInfo(&writeDescriptorSet->info.bufferInfo);
  return writeDescriptorSet;
}

std::unique_ptr<DescriptorSet::WriteDescriptorSetBuild> DescriptorSet::CreateImageSamplerWrite(
    const ImageView& samplerImageView, const Sampler& sampler, const VkImageLayout imageLayout) const {
  std::unique_ptr<WriteDescriptorSetBuild> writeDescriptorSet = std::make_unique<WriteDescriptorSetBuild>();
  writeDescriptorSet->info.imageInfo =
      DescriptorImageInfoBuilder()
          .SetImageView(samplerImageView.imageView)
          .SetSampler(sampler.sampler)
          .SetImageLayout(imageLayout)
          .BuildObject();
  writeDescriptorSet->writeBuilder =
      WriteDescriptorSetBuilder()
          .SetDstSet(descriptorSet)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetDescriptorCount(1)
          .SetPImageInfo(&writeDescriptorSet->info.imageInfo);
  return writeDescriptorSet;
}