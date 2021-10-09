#ifndef VULKAN_SRC_VULKAN_STRUCTURES_SWAPCHAINCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_SWAPCHAINCREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(SwapchainCreateInfoBuilder, VkSwapchainCreateInfoKHR,
                  VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR)
STRUCTURE_SETTER(Flags, const VkSwapchainCreateFlagsKHR, flags)
STRUCTURE_SETTER(Surface, VkSurfaceKHR, surface)
STRUCTURE_SETTER(MinImageCount, const u32, minImageCount)
STRUCTURE_SETTER(ImageFormat, const VkFormat, imageFormat)
STRUCTURE_SETTER(ImageColorSpace, const VkColorSpaceKHR, imageColorSpace)
STRUCTURE_SETTER(ImageExtent, const VkExtent2D, imageExtent)
STRUCTURE_SETTER(ImageArrayLayers, const u32, imageArrayLayers)
STRUCTURE_SETTER(ImageUsage, const VkImageUsageFlags, imageUsage)
STRUCTURE_SETTER(ImageSharingMode, const VkSharingMode, imageSharingMode)
STRUCTURE_SETTER(QueueFamilyIndexCount, const u32, queueFamilyIndexCount)
STRUCTURE_SETTER(PQueueFamilyIndices, const u32*, pQueueFamilyIndices)
STRUCTURE_SETTER(PreTransform, const VkSurfaceTransformFlagBitsKHR,
                 preTransform)
STRUCTURE_SETTER(CompositeAlpha, const VkCompositeAlphaFlagBitsKHR,
                 compositeAlpha)
STRUCTURE_SETTER(PresentMode, const VkPresentModeKHR, presentMode)
STRUCTURE_SETTER(Clipped, const VkBool32, clipped)
STRUCTURE_SETTER(OldSwapchain, VkSwapchainKHR, oldSwapchain)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_SWAPCHAINCREATEINFO_H
