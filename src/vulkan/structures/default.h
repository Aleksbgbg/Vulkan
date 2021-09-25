#ifndef VULKAN_SRC_VULKAN_STRUCTURES_DEFAULT_H
#define VULKAN_SRC_VULKAN_STRUCTURES_DEFAULT_H

#include <vulkan/vulkan_core.h>

#include "BufferCreateInfo.h"
#include "ImageCreateInfo.h"
#include "Extent3D.h"
#include "ImageViewCreateInfo.h"
#include "ImageSubresourceRange.h"
#include "Viewport.h"
#include "ImageMemoryBarrier.h"

inline const VkBufferCreateInfo BUFFER_EXCLUSIVE =
    BufferCreateInfoBuilder().SetSharingMode(VK_SHARING_MODE_EXCLUSIVE).BuildObject();

inline const VkImageCreateInfo IMAGE_2D =
    ImageCreateInfoBuilder()
        .SetImageType(VK_IMAGE_TYPE_2D)
        .SetMipLevels(1)
        .SetArrayLayers(1)
        .SetSamples(VK_SAMPLE_COUNT_1_BIT)
        .SetTiling(VK_IMAGE_TILING_OPTIMAL)
        .SetSharingMode(VK_SHARING_MODE_EXCLUSIVE)
        .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
        .BuildObject();

//inline const VkImageViewCreateInfo IMAGE_VIEW_2D =
//    ImageViewCreateInfoBuilder()
//        .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
//        .SetImageType(VK_IMAGE_TYPE_2D)
//        .SetMipLevels(1)
//        .SetArrayLayers(1)
//        .SetSamples(VK_SAMPLE_COUNT_1_BIT)
//        .SetTiling(VK_IMAGE_TILING_OPTIMAL)
//        .SetSharingMode(VK_SHARING_MODE_EXCLUSIVE)
//        .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
//        .BuildObject();

inline const VkViewport VIEWPORT_BASE = ViewportBuilder().SetMaxDepth(1.0f).BuildObject();

inline const VkOffset2D OFFSET2D_ZERO = {};

inline const VkExtent3D EXTENT3D_SINGLE_DEPTH = { .depth = 1 };

inline const VkImageSubresourceLayers SUBRESOURCE_LAYERS_COLOR_SINGLE_LAYER =
    ImageSubresourceLayersBuilder().SetAspectMask(VK_IMAGE_ASPECT_COLOR_BIT).SetLayerCount(1).BuildObject();

inline const VkImageSubresourceRange SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER =
    ImageSubresourceRangeBuilder()
        .SetAspectMask(VK_IMAGE_ASPECT_COLOR_BIT)
        .SetLayerCount(1)
        .SetLevelCount(1)
        .BuildObject();

inline const VkImageMemoryBarrier IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER =
    ImageMemoryBarrierBuilder()
        .SetSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .SetDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .BuildObject();

#endif // VULKAN_SRC_VULKAN_STRUCTURES_DEFAULT_H
