#ifndef VULKAN_SRC_VULKAN_STRUCTURES_IMAGEVIEWCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_IMAGEVIEWCREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "types.h"
#include "define_structure.h"
#include "ImageSubresourceRange.h"

STRUCTURE_BUILDER(ImageViewCreateInfoBuilder, VkImageViewCreateInfo, VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO)
  STRUCTURE_SETTER(ImageViewCreateInfoBuilder, Flags, const VkImageViewCreateFlags, flags)
  STRUCTURE_SETTER(ImageViewCreateInfoBuilder, Image, VkImage, image)
  STRUCTURE_SETTER(ImageViewCreateInfoBuilder, ViewType, const VkImageViewType, viewType)
  STRUCTURE_SETTER(ImageViewCreateInfoBuilder, Format, const VkFormat, format)
  STRUCTURE_SETTER(ImageViewCreateInfoBuilder, Components, const VkComponentMapping, components)
  STRUCTURE_SETTER(ImageViewCreateInfoBuilder, SubresourceRange, const VkImageSubresourceRange, subresourceRange)
  STRUCTURE_SETTER_BUILDER_OBJECT(ImageViewCreateInfoBuilder, SubresourceRange, ImageSubresourceRangeBuilder, subresourceRange)
END_STRUCTURE_BUILDER

#endif // VULKAN_SRC_VULKAN_STRUCTURES_IMAGEVIEWCREATEINFO_H
