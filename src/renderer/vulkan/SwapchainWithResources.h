#ifndef VULKAN_SRC_RENDERER_VULKAN_SWAPCHAINWITHRESOURCES_H_
#define VULKAN_SRC_RENDERER_VULKAN_SWAPCHAINWITHRESOURCES_H_

#include "BoundImage.h"
#include "renderer/vulkan/api/Surface.h"
#include "renderer/vulkan/api/Swapchain.h"
#include "renderer/vulkan/api/VirtualDevice.h"
#include "renderer/vulkan/api/memory/DeviceMemoryAllocator.h"

class SwapchainWithResources {
 public:
  class Initializer {
   public:
    virtual ~Initializer() = default;

    virtual Swapchain CreateSwapchain() const = 0;
    virtual Swapchain CreateSwapchain(const Swapchain& oldSwapchain) const = 0;

    virtual BoundImage CreateDepthStencilAttachment(
        const Swapchain& swapchain) = 0;
    virtual BoundImage CreateMultisamplingAttachment(
        const Swapchain& swapchain) = 0;

    virtual std::vector<Framebuffer> GetFramebuffers(
        const Swapchain& swapchain,
        const std::vector<const ImageView*>& attachments) const = 0;

    virtual Semaphore CreateSemaphore() const = 0;
  };

  struct AcquireNextImageResult {
    u32 imageIndex;
    VkResult status;
    const Semaphore& semaphore;
    const Framebuffer& framebuffer;
  };

  SwapchainWithResources() = default;
  SwapchainWithResources(Initializer& initializer);

  SwapchainWithResources RecreateSwapchain(Initializer& initializer) const;

  u32 GetImageCount() const;
  VkExtent2D GetImageExtent() const;

  AcquireNextImageResult AcquireNextImage();

  void Present(const Queue& queue,
               const SynchronisationPack& synchronisation) const;

 private:
  SwapchainWithResources(Swapchain inSwapchain, Initializer& initializer);

 private:
  Swapchain swapchain;

  BoundImage depthStencil;
  ImageView depthStencilView;

  BoundImage multisampling;
  ImageView multisamplingImageView;

  std::vector<Framebuffer> swapchainFramebuffers;

  u32 imagesInFlight;

  struct InFlightImage {
    Semaphore acquireImageSemaphore;
  };
  std::vector<InFlightImage> imagesInFlightSynchronisation;
  u32 currentInFlightImage;
  u32 currentImageIndex;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_SWAPCHAINWITHRESOURCES_H_
