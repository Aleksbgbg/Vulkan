#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORIMAGEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORIMAGEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_TYPE(DescriptorImageInfoBuilder, VkDescriptorImageInfo)
  STRUCTURE_SETTER(DescriptorImageInfoBuilder, Sampler, VkSampler, sampler)
  STRUCTURE_SETTER(DescriptorImageInfoBuilder, ImageView, VkImageView, imageView)
  STRUCTURE_SETTER(DescriptorImageInfoBuilder, ImageLayout, VkImageLayout, imageLayout)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORIMAGEINFO_H
