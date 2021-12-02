#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETALLOCATEINFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETALLOCATEINFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(DescriptorSetAllocateInfoBuilder, VkDescriptorSetAllocateInfo,
                  VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO)
STRUCTURE_SETTER(DescriptorPool, VkDescriptorPool, descriptorPool)
STRUCTURE_SETTER(DescriptorSetCount, u32, descriptorSetCount)
STRUCTURE_SETTER(PSetLayouts, const VkDescriptorSetLayout*, pSetLayouts)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORSETALLOCATEINFO_H_
