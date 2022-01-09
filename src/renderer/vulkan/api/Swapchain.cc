#include "Swapchain.h"

#include <algorithm>
#include <array>
#include <iterator>

#include "error.h"
#include "util.h"

namespace vk {

Swapchain::Swapchain() : swapchain(nullptr) {}

Swapchain::Swapchain(VkDevice device, const Swapchain& oldSwapchain,
                     SwapchainCreateInfoBuilder& infoBuilder)
    : Swapchain(device, infoBuilder.SetOldSwapchain(oldSwapchain.swapchain)) {}

Swapchain::Swapchain(VkDevice device,
                     const SwapchainCreateInfoBuilder& infoBuilder)
    : device(device) {
  const VkSwapchainCreateInfoKHR swapchainCreateInfo = infoBuilder;
  imageFormat = swapchainCreateInfo.imageFormat;
  imageExtent = swapchainCreateInfo.imageExtent;
  PROCEED_ON_VALID_RESULT(
      vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain));
  images = LoadArray([this](u32* const count, VkImage* const images) {
    vkGetSwapchainImagesKHR(this->device, swapchain, count, images);
  });
  std::transform(images.begin(), images.end(), std::back_inserter(imageViews),
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

Swapchain::Swapchain(Swapchain&& other) noexcept
    : device(other.device),
      swapchain(other.swapchain),
      images(std::move(other.images)),
      imageViews(std::move(other.imageViews)),
      imageFormat(other.imageFormat),
      imageExtent(other.imageExtent) {
  other.swapchain = nullptr;
}

Swapchain::~Swapchain() {
  if (swapchain != nullptr) {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
  }
}

Swapchain& Swapchain::operator=(Swapchain&& other) noexcept {
  std::swap(device, other.device);
  std::swap(swapchain, other.swapchain);
  images = std::move(other.images);
  imageViews = std::move(other.imageViews);
  imageFormat = other.imageFormat;
  imageExtent = other.imageExtent;
  return *this;
}

VkFormat Swapchain::GetImageFormat() const {
  return imageFormat;
}

VkExtent2D Swapchain::GetImageExtent() const {
  return imageExtent;
}

Swapchain::AcquireNextImageResult Swapchain::AcquireNextImage(
    const SynchronisationPack& synchronisation) const {
  u32 imageIndex;
  const VkResult status = vkAcquireNextImageKHR(
      device, swapchain, UINT64_MAX,
      synchronisation.GetSignalSemaphore()->Raw(), VK_NULL_HANDLE, &imageIndex);

  if ((status != VK_ERROR_OUT_OF_DATE_KHR) && (status != VK_SUBOPTIMAL_KHR)) {
    PROCEED_ON_VALID_RESULT(status)
  }

  return {.imageIndex = imageIndex, .status = status};
}

u32 Swapchain::GetImageCount() const {
  return images.size();
}

std::vector<Framebuffer> Swapchain::GetFramebuffers(
    const RenderPass& renderPass) const {
  return GetFramebuffers(renderPass, {});
}

std::vector<Framebuffer> Swapchain::GetFramebuffers(
    const RenderPass& renderPass,
    const std::vector<const ImageView*>& attachments) const {
  std::vector<VkImageView> imageViewAttachments(attachments.size() + 1);
  std::transform(
      attachments.begin(), attachments.end(), imageViewAttachments.begin() + 1,
      [](const ImageView* const imageView) { return imageView->imageView; });

  std::vector<Framebuffer> framebuffers(imageViews.size());
  std::transform(
      imageViews.begin(), imageViews.end(), framebuffers.begin(),
      [this, &renderPass, &imageViewAttachments](const ImageView& imageView) {
        imageViewAttachments[0] = imageView.imageView;
        return Framebuffer(device,
                           FramebufferCreateInfoBuilder()
                               .SetRenderPass(renderPass.renderPass)
                               .SetAttachmentCount(imageViewAttachments.size())
                               .SetPAttachments(imageViewAttachments.data())
                               .SetWidth(imageExtent.width)
                               .SetHeight(imageExtent.height)
                               .SetLayers(1));
      });

  return framebuffers;
}

}  // namespace vk
