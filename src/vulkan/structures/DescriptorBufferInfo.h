#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORBUFFERINFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORBUFFERINFO_H_

#include <vulkan/vulkan.h>

#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER_NO_STYPE(DescriptorBufferInfoBuilder, VkDescriptorBufferInfo)
STRUCTURE_SETTER(Buffer, VkBuffer, buffer)
STRUCTURE_SETTER(Offset, VkDeviceSize, offset)
STRUCTURE_SETTER(Range, VkDeviceSize, range)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_DESCRIPTORBUFFERINFO_H_
