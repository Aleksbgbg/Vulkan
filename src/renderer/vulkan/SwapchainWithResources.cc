#include "SwapchainWithResources.h"

SwapchainWithResources::SwapchainWithResources(Initializer& initializer)
    : SwapchainWithResources(initializer.CreateSwapchain(), initializer) {}

SwapchainWithResources::SwapchainWithResources(vk::Swapchain inSwapchain,
                                               Initializer& initializer)
    : swapchain(std::move(inSwapchain)),
      depthStencil(initializer.CreateDepthStencilAttachment(swapchain)),
      depthStencilView(depthStencil.CreateView(
          ImageViewCreateInfoBuilder()
              .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
              .SetSubresourceRange(ImageSubresourceRangeBuilder()
                                       .SetAspectMask(VK_IMAGE_ASPECT_DEPTH_BIT)
                                       .SetLevelCount(1)
                                       .SetLayerCount(1)))),
      multisampling(initializer.CreateMultisamplingAttachment(swapchain)),
      multisamplingImageView(multisampling.CreateView(
          ImageViewCreateInfoBuilder()
              .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
              .SetSubresourceRange(ImageSubresourceRangeBuilder()
                                       .SetAspectMask(VK_IMAGE_ASPECT_COLOR_BIT)
                                       .SetLevelCount(1)
                                       .SetLayerCount(1)))),
      swapchainFramebuffers(initializer.GetFramebuffers(
          swapchain, {&multisamplingImageView, &depthStencilView})),
      imagesInFlight(swapchain.GetImageCount()),
      imagesInFlightSynchronisation(imagesInFlight),
      currentInFlightImage(0),
      currentImageIndex(0) {
  for (u32 inFlightImage = 0; inFlightImage < imagesInFlight; ++inFlightImage) {
    imagesInFlightSynchronisation[inFlightImage] = std::move<InFlightImage>(
        {.acquireImageSemaphore = initializer.CreateSemaphore()});
  }
}

SwapchainWithResources SwapchainWithResources::RecreateSwapchain(
    Initializer& initializer) const {
  return SwapchainWithResources(initializer.CreateSwapchain(swapchain),
                                initializer);
}

u32 SwapchainWithResources::GetImageCount() const {
  return swapchain.GetImageCount();
}

VkExtent2D SwapchainWithResources::GetImageExtent() const {
  return swapchain.GetImageExtent();
}

SwapchainWithResources::AcquireNextImageResult
SwapchainWithResources::AcquireNextImage() {
  const u32 inFlightImage = currentInFlightImage;
  currentInFlightImage = (currentInFlightImage + 1) % imagesInFlight;

  const vk::Semaphore& acquireImageSemaphore =
      imagesInFlightSynchronisation[inFlightImage].acquireImageSemaphore;

  const vk::Swapchain::AcquireNextImageResult acquireNextImageResult =
      swapchain.AcquireNextImage(
          vk::SynchronisationPack().SetSignalSemaphore(&acquireImageSemaphore));
  currentImageIndex = acquireNextImageResult.imageIndex;

  return AcquireNextImageResult{
      .imageIndex = acquireNextImageResult.imageIndex,
      .status = acquireNextImageResult.status,
      .semaphore = acquireImageSemaphore,
      .framebuffer = swapchainFramebuffers[inFlightImage]};
}

void SwapchainWithResources::Present(
    const vk::Queue& queue,
    const vk::SynchronisationPack& synchronisation) const {
  queue.Present(swapchain, currentImageIndex, synchronisation);
}
