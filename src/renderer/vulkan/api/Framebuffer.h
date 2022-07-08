#ifndef SRC_RENDERER_VULKAN_API_FRAMEBUFFER_H_
#define SRC_RENDERER_VULKAN_API_FRAMEBUFFER_H_

#include <vulkan/vulkan.h>

#include "renderer/vulkan/api/structures/FramebufferCreateInfo.h"

namespace vk {

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

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_FRAMEBUFFER_H_
