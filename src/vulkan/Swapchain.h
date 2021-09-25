#ifndef VULKAN_SRC_VULKAN_SWAPCHAIN_H
#define VULKAN_SRC_VULKAN_SWAPCHAIN_H

#include <vector>

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/SwapchainCreateInfo.h"
#include "Framebuffer.h"
#include "RenderPass.h"
#include "Image.h"
#include "Fence.h"
#include "Semaphore.h"
#include "SynchronisationPack.h"

class Swapchain {
  friend class Queue;
public:
  Swapchain() = default;
  Swapchain(VkDevice device, SwapchainCreateInfoBuilder& infoBuilder);

  Swapchain(const Swapchain&) = delete;
  Swapchain(Swapchain&& other) noexcept
    :
    device(other.device),
    swapchain(other.swapchain),
    images(std::move(other.images)),
    imageViews(std::move(other.imageViews)),
    imageFormat(other.imageFormat),
    imageExtent(other.imageExtent) {
    other.swapchain = nullptr;
  }

  ~Swapchain();

  Swapchain& operator=(const Swapchain&) = delete;
  Swapchain& operator=(Swapchain&& other) noexcept {
    device = other.device;
    std::swap(swapchain, other.swapchain);
    images = std::move(other.images);
    imageViews = std::move(other.imageViews);
    imageFormat = other.imageFormat;
    imageExtent = other.imageExtent;
    return *this;
  }

  VkFormat GetImageFormat() const;
  VkExtent2D GetImageExtent() const;

  struct AcquireNextImageResult {
    u32 imageIndex;
    VkResult status;
  };
  AcquireNextImageResult AcquireNextImage(const SynchronisationPack& synchronisation) const;

  u32 GetImageCount() const;

  std::vector<Framebuffer> GetFramebuffers(RenderPass& renderPass, ImageView& depthImageView);

private:
  VkDevice device;
  VkSwapchainKHR swapchain = nullptr;
  std::vector<VkImage> images;
  std::vector<ImageView> imageViews;
  VkFormat imageFormat;
  VkExtent2D imageExtent;
};

#endif // VULKAN_SRC_VULKAN_SWAPCHAIN_H
