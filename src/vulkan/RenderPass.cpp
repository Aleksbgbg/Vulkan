#include "RenderPass.h"

#include "error.h"

RenderPass::RenderPass(VkDevice device,
                       RenderPassCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(
      vkCreateRenderPass(device, infoBuilder.Build(), nullptr, &renderPass));
}

RenderPass::~RenderPass() {
  if (renderPass != nullptr) {
    vkDestroyRenderPass(device, renderPass, nullptr);
  }
}
