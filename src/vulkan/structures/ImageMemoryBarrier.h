#ifndef VULKAN_SRC_VULKAN_STRUCTURES_IMAGEMEMORYBARRIER_H
#define VULKAN_SRC_VULKAN_STRUCTURES_IMAGEMEMORYBARRIER_H

#include <vulkan/vulkan_core.h>

#include "ImageSubresourceRange.h"
#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(ImageMemoryBarrierBuilder, VkImageMemoryBarrier,
                  VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER)
STRUCTURE_SETTER(ImageMemoryBarrierBuilder, SrcAccessMask, VkAccessFlags,
                 srcAccessMask)
STRUCTURE_SETTER(ImageMemoryBarrierBuilder, DstAccessMask, VkAccessFlags,
                 dstAccessMask)
STRUCTURE_SETTER(ImageMemoryBarrierBuilder, OldLayout, VkImageLayout, oldLayout)
STRUCTURE_SETTER(ImageMemoryBarrierBuilder, NewLayout, VkImageLayout, newLayout)
STRUCTURE_SETTER(ImageMemoryBarrierBuilder, SrcQueueFamilyIndex, u32,
                 srcQueueFamilyIndex)
STRUCTURE_SETTER(ImageMemoryBarrierBuilder, DstQueueFamilyIndex, u32,
                 dstQueueFamilyIndex)
STRUCTURE_SETTER(ImageMemoryBarrierBuilder, Image, VkImage, image)
STRUCTURE_SETTER(ImageMemoryBarrierBuilder, SubresourceRange,
                 VkImageSubresourceRange, subresourceRange)
STRUCTURE_SETTER_BUILDER_OBJECT(ImageMemoryBarrierBuilder, SubresourceRange,
                                ImageSubresourceRangeBuilder, subresourceRange)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_IMAGEMEMORYBARRIER_H
