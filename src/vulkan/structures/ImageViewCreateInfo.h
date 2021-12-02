#ifndef VULKAN_SRC_VULKAN_STRUCTURES_IMAGEVIEWCREATEINFO_H_
#define VULKAN_SRC_VULKAN_STRUCTURES_IMAGEVIEWCREATEINFO_H_

#include <vulkan/vulkan.h>

#include "ImageSubresourceRange.h"
#include "define_structure.h"
#include "util/types.h"

STRUCTURE_BUILDER(ImageViewCreateInfoBuilder, VkImageViewCreateInfo,
                  VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO)
STRUCTURE_SETTER(Flags, const VkImageViewCreateFlags, flags)
STRUCTURE_SETTER(Image, VkImage, image)
STRUCTURE_SETTER(ViewType, const VkImageViewType, viewType)
STRUCTURE_SETTER(Format, const VkFormat, format)
STRUCTURE_SETTER(Components, const VkComponentMapping, components)
STRUCTURE_SETTER(SubresourceRange, const VkImageSubresourceRange,
                 subresourceRange)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_IMAGEVIEWCREATEINFO_H_
