#include "Framebuffer.h"

#include "error.h"

Framebuffer::Framebuffer(VkDevice device,
                         FramebufferCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(
      vkCreateFramebuffer(device, infoBuilder.Build(), nullptr, &framebuffer));
}

Framebuffer::~Framebuffer() {
  if (framebuffer != nullptr) {
    vkDestroyFramebuffer(device, framebuffer, nullptr);
  }
}
