#ifndef SRC_RENDERER_VULKAN_SWAPCHAINWITHRESOURCES_H_
#define SRC_RENDERER_VULKAN_SWAPCHAINWITHRESOURCES_H_

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

    virtual vk::Swapchain CreateSwapchain() const = 0;
    virtual vk::Swapchain CreateSwapchain(
        const vk::Swapchain& oldSwapchain) const = 0;

    virtual BoundImage CreateDepthStencilAttachment(
        const vk::Swapchain& swapchain) = 0;
    virtual BoundImage CreateMultisamplingAttachment(
        const vk::Swapchain& swapchain) = 0;

    virtual std::vector<vk::Framebuffer> GetFramebuffers(
        const vk::Swapchain& swapchain,
        const std::vector<const vk::ImageView*>& attachments) const = 0;

    virtual vk::Semaphore CreateSemaphore() const = 0;
  };

  struct AcquireNextImageResult {
    u32 imageIndex;
    VkResult status;
    const vk::Semaphore& semaphore;
    const vk::Framebuffer& framebuffer;
  };

  SwapchainWithResources() = default;
  SwapchainWithResources(Initializer& initializer);

  SwapchainWithResources RecreateSwapchain(Initializer& initializer) const;

  u32 GetImageCount() const;
  VkExtent2D GetImageExtent() const;

  AcquireNextImageResult AcquireNextImage();

  void Present(const vk::Queue& queue,
               const vk::SynchronisationPack& synchronisation) const;

 private:
  SwapchainWithResources(vk::Swapchain inSwapchain, Initializer& initializer);

 private:
  vk::Swapchain swapchain;

  BoundImage depthStencil;
  vk::ImageView depthStencilView;

  BoundImage multisampling;
  vk::ImageView multisamplingImageView;

  std::vector<vk::Framebuffer> swapchainFramebuffers;

  u32 imagesInFlight;

  struct InFlightImage {
    vk::Semaphore acquireImageSemaphore;
  };
  std::vector<InFlightImage> imagesInFlightSynchronisation;
  u32 currentInFlightImage;
  u32 currentImageIndex;
};

#endif  // SRC_RENDERER_VULKAN_SWAPCHAINWITHRESOURCES_H_
