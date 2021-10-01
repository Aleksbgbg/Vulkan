#ifndef VULKAN_SRC_VULKAN_RENDERPASS_H
#define VULKAN_SRC_VULKAN_RENDERPASS_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/RenderPassCreateInfo.h"

class RenderPass {
  friend class Swapchain;
  friend class Pipeline;
  friend class CommandBuffer;
  friend class ImGuiInstance;
public:
  VULKAN_OBJECT_MOVABLE_ROOT(RenderPass, device, renderPass)

  RenderPass() = default;
  RenderPass(VkDevice device, RenderPassCreateInfoBuilder& infoBuilder);

  RenderPass(const RenderPass&) = delete;

  ~RenderPass();

  RenderPass& operator=(const RenderPass&) = delete;

private:
  VkDevice device;
  VkRenderPass renderPass = nullptr;
};;

#endif // VULKAN_SRC_VULKAN_RENDERPASS_H
