#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORIMAGEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORIMAGEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER_NO_STYPE(DescriptorImageInfoBuilder, VkDescriptorImageInfo)
STRUCTURE_SETTER(Sampler, VkSampler, sampler)
STRUCTURE_SETTER(ImageView, VkImageView, imageView)
STRUCTURE_SETTER(ImageLayout, VkImageLayout, imageLayout)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORIMAGEINFO_H_
