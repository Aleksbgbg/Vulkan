#include "Framebuffer.h"

#include <utility>

#include "error.h"

namespace vk {

Framebuffer::Framebuffer() : framebuffer(nullptr) {}

Framebuffer::Framebuffer(VkDevice device,
                         const FramebufferCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(
      vkCreateFramebuffer(device, infoBuilder.Build(), nullptr, &framebuffer));
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
    : device(other.device), framebuffer(other.framebuffer) {
  other.framebuffer = nullptr;
}

Framebuffer::~Framebuffer() {
  if (framebuffer != nullptr) {
    vkDestroyFramebuffer(device, framebuffer, nullptr);
  }
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
  this->~Framebuffer();
  new (this) Framebuffer(std::move(other));
  return *this;
}

}  // namespace vk
