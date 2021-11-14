#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLSIZE_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLSIZE_H

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(DescriptorPoolSizeBuilder, VkDescriptorPoolSize)
STRUCTURE_SETTER(Type, VkDescriptorType, type)
STRUCTURE_SETTER(DescriptorCount, u32, descriptorCount)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORPOOLSIZE_H
