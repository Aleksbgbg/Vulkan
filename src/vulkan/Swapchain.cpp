#include "Swapchain.h"

#include <algorithm>
#include <array>
#include <iterator>

#include "error.h"
#include "util.h"

Swapchain::Swapchain(VkDevice device, SwapchainCreateInfoBuilder& infoBuilder) : device(device) {
  VkSwapchainCreateInfoKHR swapchainCreateInfo = infoBuilder.BuildObject();
  imageFormat = swapchainCreateInfo.imageFormat;
  imageExtent = swapchainCreateInfo.imageExtent;
  PROCEED_ON_VALID_RESULT(vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain))
  images = LoadArray([this](u32* const count, VkImage* const images) {
    vkGetSwapchainImagesKHR(this->device, swapchain, count, images);
  });
  std::transform(
      images.begin(),
      images.end(),
      std::back_inserter(imageViews),
      [this](VkImage image) {
        return ImageView(
            this->device,
            ImageViewCreateInfoBuilder()
                .SetImage(image)
                .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
                .SetFormat(imageFormat)
                .SetSubresourceRange(
                    ImageSubresourceRangeBuilder()
                        .SetAspectMask(VK_IMAGE_ASPECT_COLOR_BIT)
                        .SetLevelCount(1)
                        .SetLayerCount(1)));
      });
}

Swapchain::~Swapchain() {
  if (swapchain != nullptr) {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
  }
}

VkFormat Swapchain::GetImageFormat() const {
  return imageFormat;
}

VkExtent2D Swapchain::GetImageExtent() const {
  return imageExtent;
}

Swapchain::AcquireNextImageResult Swapchain::AcquireNextImage(const SynchronisationPack& synchronisation) const {
  u32 imageIndex;
  VkResult status =
      vkAcquireNextImageKHR(
          device, swapchain, UINT64_MAX, synchronisation.GetSignalSemaphore()->Raw(), VK_NULL_HANDLE, &imageIndex);

  if (!((status == VK_ERROR_OUT_OF_DATE_KHR) || (status == VK_SUBOPTIMAL_KHR))) {
    PROCEED_ON_VALID_RESULT(status)
  }

  return {
    .imageIndex = imageIndex,
    .status = status
  };
}

u32 Swapchain::GetImageCount() const {
  return images.size();
}

std::vector<Framebuffer> Swapchain::GetFramebuffers(
    RenderPass& renderPass, ImageView& depthImageView) {
  std::vector<Framebuffer> framebuffers;
  std::transform(
      imageViews.begin(),
      imageViews.end(),
      std::back_inserter(framebuffers),
      [this, &renderPass, &depthImageView](ImageView& imageView) {
        std::array<VkImageView, 2> attachments{
          imageView.imageView, depthImageView.imageView
        };

        return Framebuffer(
            device,
            FramebufferCreateInfoBuilder()
                .SetRenderPass(renderPass.renderPass)
                .SetAttachmentCount(attachments.size())
                .SetPAttachments(attachments.data())
                .SetWidth(imageExtent.width)
                .SetHeight(imageExtent.height)
                .SetLayers(1));
      });
  return framebuffers;
}
