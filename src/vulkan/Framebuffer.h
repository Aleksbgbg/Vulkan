#ifndef VULKAN_SRC_VULKAN_FRAMEBUFFER_H
#define VULKAN_SRC_VULKAN_FRAMEBUFFER_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/FramebufferCreateInfo.h"

class Framebuffer {
  friend class CommandBuffer;
public:
  VULKAN_OBJECT_MOVABLE_ROOT(Framebuffer, device, framebuffer)

  Framebuffer() = default;
  Framebuffer(VkDevice device, FramebufferCreateInfoBuilder& infoBuilder);

  Framebuffer(const Framebuffer&) = delete;

  ~Framebuffer();

  Framebuffer& operator=(const Framebuffer&) = delete;

private:
  VkDevice device;
  VkFramebuffer framebuffer = nullptr;
};

#endif // VULKAN_SRC_VULKAN_FRAMEBUFFER_H
