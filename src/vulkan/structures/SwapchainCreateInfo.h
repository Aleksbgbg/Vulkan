#ifndef VULKAN_SRC_VULKAN_STRUCTURES_SWAPCHAINCREATEINFO_H
#define VULKAN_SRC_VULKAN_STRUCTURES_SWAPCHAINCREATEINFO_H

#include <vulkan/vulkan_core.h>

#include "define_structure.h"
#include "types.h"

STRUCTURE_BUILDER(SwapchainCreateInfoBuilder, VkSwapchainCreateInfoKHR,
                  VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, Flags,
                 const VkSwapchainCreateFlagsKHR, flags)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, Surface, VkSurfaceKHR, surface)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, MinImageCount, const u32,
                 minImageCount)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, ImageFormat, const VkFormat,
                 imageFormat)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, ImageColorSpace,
                 const VkColorSpaceKHR, imageColorSpace)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, ImageExtent, const VkExtent2D,
                 imageExtent)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, ImageArrayLayers, const u32,
                 imageArrayLayers)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, ImageUsage,
                 const VkImageUsageFlags, imageUsage)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, ImageSharingMode,
                 const VkSharingMode, imageSharingMode)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, QueueFamilyIndexCount, const u32,
                 queueFamilyIndexCount)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, PQueueFamilyIndices, const u32*,
                 pQueueFamilyIndices)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, PreTransform,
                 const VkSurfaceTransformFlagBitsKHR, preTransform)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, CompositeAlpha,
                 const VkCompositeAlphaFlagBitsKHR, compositeAlpha)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, PresentMode,
                 const VkPresentModeKHR, presentMode)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, Clipped, const VkBool32, clipped)
STRUCTURE_SETTER(SwapchainCreateInfoBuilder, OldSwapchain, VkSwapchainKHR,
                 oldSwapchain)
END_STRUCTURE_BUILDER

#endif  // VULKAN_SRC_VULKAN_STRUCTURES_SWAPCHAINCREATEINFO_H
