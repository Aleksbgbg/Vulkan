#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORSETALLOCATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORSETALLOCATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(DescriptorSetAllocateInfoBuilder, VkDescriptorSetAllocateInfo,
                  VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO)
STRUCTURE_SETTER(DescriptorPool, VkDescriptorPool, descriptorPool)
STRUCTURE_SETTER(DescriptorSetCount, u32, descriptorSetCount)
STRUCTURE_SETTER(PSetLayouts, const VkDescriptorSetLayout*, pSetLayouts)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORSETALLOCATEINFO_H_
