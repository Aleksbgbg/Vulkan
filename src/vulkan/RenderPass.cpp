#include "RenderPass.h"

#include "error.h"

RenderPass::RenderPass() : renderPass(nullptr) {}

RenderPass::RenderPass(RenderPass&& other) noexcept
    : device(other.device), renderPass(other.renderPass) {
  other.renderPass = nullptr;
}

RenderPass::RenderPass(VkDevice device,
                       const RenderPassCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(
      vkCreateRenderPass(device, infoBuilder.Build(), nullptr, &renderPass));
}

RenderPass::~RenderPass() {
  if (renderPass != nullptr) {
    vkDestroyRenderPass(device, renderPass, nullptr);
  }
}

RenderPass& RenderPass::operator=(RenderPass&& other) noexcept {
  std::swap(device, other.device);
  std::swap(renderPass, other.renderPass);
  return *this;
}
