#ifndef SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORPOOLCREATEINFO_H_
#define SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORPOOLCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "DescriptorPoolSize.h"
#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(DescriptorPoolCreateInfoBuilder, VkDescriptorPoolCreateInfo,
                  VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO)
STRUCTURE_SETTER(Flags, VkDescriptorPoolCreateFlags, flags)
STRUCTURE_SETTER(MaxSets, u32, maxSets)
STRUCTURE_SETTER(PoolSizeCount, u32, poolSizeCount)
STRUCTURE_SETTER(PPoolSizes, const VkDescriptorPoolSize*, pPoolSizes)
STRUCTURE_SETTER_POINTER_FROM_BUILDER(PPoolSizes, DescriptorPoolSizeBuilder,
                                      pPoolSizes)
END_STRUCTURE_BUILDER

#endif  // SRC_RENDERER_VULKAN_API_STRUCTURES_DESCRIPTORPOOLCREATEINFO_H_
