#ifndef VULKAN_SRC_SWAPCHAINWITHRESOURCES_H
#define VULKAN_SRC_SWAPCHAINWITHRESOURCES_H

#include "game/rendering/ImageWithMemory.h"
#include "memory/DeviceMemoryAllocator.h"
#include "vulkan/Surface.h"
#include "vulkan/Swapchain.h"
#include "vulkan/VirtualDevice.h"

class SwapchainWithResources {
 public:
  struct AcquireNextImageResult {
    u32 imageIndex;
    VkResult status;
    const Semaphore& semaphore;
  };

  SwapchainWithResources() = default;
  SwapchainWithResources(const VirtualDevice& virtualDevice,
                         const Surface& windowSurface,
                         DeviceMemoryAllocator& deviceAllocator,
                         const SwapchainCreateInfoBuilder createInfoBuilder,
                         const VkFormat depthStencilFormat,
                         const VkSampleCountFlagBits samples,
                         const RenderPass& renderPass);

  SwapchainWithResources RecreateSwapchain(
      const VirtualDevice& virtualDevice, const Surface& windowSurface,
      DeviceMemoryAllocator& deviceAllocator, const RenderPass& renderPass,
      const VkExtent2D extent) const;

  u32 GetImageCount() const;
  VkExtent2D GetImageExtent() const;

  AcquireNextImageResult AcquireNextImage();

  const Framebuffer& CurrentFramebuffer() const;

  void Present(const Queue& queue,
               const SynchronisationPack& synchronisation) const;

  void MoveToNextFrame();

 private:
  SwapchainCreateInfoBuilder createInfoBuilder;
  VkFormat depthStencilFormat;
  VkSampleCountFlagBits samples;

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

#endif  // VULKAN_SRC_SWAPCHAINWITHRESOURCES_H
