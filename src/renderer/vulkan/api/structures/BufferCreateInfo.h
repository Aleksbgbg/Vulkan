#ifndef VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_BUFFERCREATEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_BUFFERCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "core/types.h"
#include "define_structure.h"

STRUCTURE_BUILDER(BufferCreateInfoBuilder, VkBufferCreateInfo,
                  VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO)
STRUCTURE_SETTER(Size, const VkDeviceSize, size)
STRUCTURE_SETTER(Usage, const VkBufferUsageFlags, usage)
STRUCTURE_SETTER(SharingMode, const VkSharingMode, sharingMode)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_STRUCTURES_BUFFERCREATEINFO_H_
