#include "SwapchainWithResources.h"

#include <stdexcept>

#include "vulkan/structures/default.h"

SwapchainWithResources::SwapchainWithResources(
    const VirtualDevice& virtualDevice, const Surface& windowSurface,
    DeviceMemoryAllocator& deviceAllocator,
    const SwapchainCreateInfoBuilder createInfoBuilder,
    const VkFormat depthStencilFormat, const VkSampleCountFlagBits samples,
    const RenderPass& renderPass)
    : createInfoBuilder(createInfoBuilder),
      depthStencilFormat(depthStencilFormat),
      samples(samples),
      swapchain(virtualDevice.CreateSwapchain(windowSurface,
                                              std::move(createInfoBuilder))),
      swapchainFramebuffers(),
      imagesInFlight(swapchain.GetImageCount()),
      imagesInFlightSynchronisation(),
      currentInFlightImage(0),
      currentImageIndex(0) {
  const VkExtent2D swapchainExtent = swapchain.GetImageExtent();
  depthStencil.image = virtualDevice.CreateImage(
      ImageCreateInfoBuilder(IMAGE_2D)
          .SetFormat(depthStencilFormat)
          .SetSamples(samples)
          .SetExtent(Extent3DBuilder(swapchainExtent).SetDepth(1))
          .SetUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
  depthStencil.memory = deviceAllocator.BindMemory(
      depthStencil.image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  depthStencilView = depthStencil.image.CreateView(
      ImageViewCreateInfoBuilder()
          .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
          .SetSubresourceRange(ImageSubresourceRangeBuilder()
                                   .SetAspectMask(VK_IMAGE_ASPECT_DEPTH_BIT)
                                   .SetLevelCount(1)
                                   .SetLayerCount(1)));

  multisampling.image = virtualDevice.CreateImage(
      ImageCreateInfoBuilder(IMAGE_2D)
          .SetFormat(swapchain.GetImageFormat())
          .SetExtent(Extent3DBuilder(swapchainExtent).SetDepth(1))
          .SetSamples(samples)
          .SetUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT));
  multisampling.memory = deviceAllocator.BindMemory(
      multisampling.image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  multisamplingImageView = multisampling.image.CreateView(
      ImageViewCreateInfoBuilder()
          .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
          .SetSubresourceRange(ImageSubresourceRangeBuilder()
                                   .SetAspectMask(VK_IMAGE_ASPECT_COLOR_BIT)
                                   .SetLevelCount(1)
                                   .SetLayerCount(1)));

  swapchainFramebuffers = swapchain.GetFramebuffers(
      renderPass, {&multisamplingImageView, &depthStencilView});

  for (u32 inFlightImage = 0; inFlightImage < imagesInFlight; ++inFlightImage) {
    imagesInFlightSynchronisation.emplace_back(std::move<InFlightImage>(
        {.acquireImageSemaphore = virtualDevice.CreateSemaphore()}));
  }
}

SwapchainWithResources SwapchainWithResources::RecreateSwapchain(
    const VirtualDevice& virtualDevice, const Surface& windowSurface,
    DeviceMemoryAllocator& deviceAllocator, const RenderPass& renderPass,
    const VkExtent2D extent) const {
  SwapchainCreateInfoBuilder newSwapchainBuilder = createInfoBuilder;
  newSwapchainBuilder.SetImageExtent(extent);
  return SwapchainWithResources(virtualDevice, windowSurface, deviceAllocator,
                                newSwapchainBuilder, depthStencilFormat,
                                samples, renderPass);
}

u32 SwapchainWithResources::GetImageCount() const {
  return swapchain.GetImageCount();
}

VkExtent2D SwapchainWithResources::GetImageExtent() const {
  return swapchain.GetImageExtent();
}

SwapchainWithResources::AcquireNextImageResult
SwapchainWithResources::AcquireNextImage() {
  const Semaphore& acquireImageSemaphore =
      imagesInFlightSynchronisation[currentInFlightImage].acquireImageSemaphore;

  const Swapchain::AcquireNextImageResult acquireNextImageResult =
      swapchain.AcquireNextImage(
          SynchronisationPack().SetSignalSemaphore(&acquireImageSemaphore));
  currentImageIndex = acquireNextImageResult.imageIndex;

  return AcquireNextImageResult{.imageIndex = acquireNextImageResult.imageIndex,
                                .status = acquireNextImageResult.status,
                                .semaphore = acquireImageSemaphore};
}

const Framebuffer& SwapchainWithResources::CurrentFramebuffer() const {
  return swapchainFramebuffers[currentInFlightImage];
}

void SwapchainWithResources::Present(
    const Queue& queue, const SynchronisationPack& synchronisation) const {
  queue.Present(swapchain, currentImageIndex, synchronisation);
}

void SwapchainWithResources::MoveToNextFrame() {
  currentInFlightImage = (currentInFlightImage + 1) % imagesInFlight;
}
