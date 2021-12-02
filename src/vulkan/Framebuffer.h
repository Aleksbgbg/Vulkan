#ifndef VULKAN_SRC_VULKAN_FRAMEBUFFER_H_
#define VULKAN_SRC_VULKAN_FRAMEBUFFER_H_

#include <vulkan/vulkan.h>

#include "structures/FramebufferCreateInfo.h"

class Framebuffer {
  friend class CommandBuffer;

 public:
  Framebuffer();
  Framebuffer(VkDevice device, const FramebufferCreateInfoBuilder& infoBuilder);

  Framebuffer(const Framebuffer&) = delete;
  Framebuffer(Framebuffer&& other) noexcept;

  ~Framebuffer();

  Framebuffer& operator=(const Framebuffer&) = delete;
  Framebuffer& operator=(Framebuffer&& other) noexcept;

 private:
  VkDevice device;
  VkFramebuffer framebuffer;
};

#endif  // VULKAN_SRC_VULKAN_FRAMEBUFFER_H_
