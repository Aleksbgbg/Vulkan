#ifndef VULKAN_SRC_SWAPCHAINWITHRESOURCES_H_
#define VULKAN_SRC_SWAPCHAINWITHRESOURCES_H_

#include "ImageWithMemory.h"
#include "memory/DeviceMemoryAllocator.h"
#include "vulkan/Surface.h"
#include "vulkan/Swapchain.h"
#include "vulkan/VirtualDevice.h"

class SwapchainWithResources {
 public:
  class Initializer {
   public:
    virtual ~Initializer() = default;

    virtual Swapchain CreateSwapchain() const = 0;
    virtual Swapchain CreateSwapchain(const Swapchain& oldSwapchain) const = 0;

    virtual ImageWithMemory CreateDepthStencilAttachment(
        const Swapchain& swapchain) = 0;
    virtual ImageWithMemory CreateMultisamplingAttachment(
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
  };

  SwapchainWithResources() = default;
  SwapchainWithResources(Initializer& initializer);

  SwapchainWithResources RecreateSwapchain(Initializer& initializer) const;

  u32 GetImageCount() const;
  VkExtent2D GetImageExtent() const;

  AcquireNextImageResult AcquireNextImage();

  const Framebuffer& CurrentFramebuffer() const;

  void Present(const Queue& queue,
               const SynchronisationPack& synchronisation) const;

  void MoveToNextFrame();

 private:
  SwapchainWithResources(Swapchain inSwapchain, Initializer& initializer);

 private:
  Swapchain swapchain;

  ImageWithMemory depthStencil;
  ImageView depthStencilView;

  ImageWithMemory multisampling;
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

#endif  // VULKAN_SRC_SWAPCHAINWITHRESOURCES_H_
