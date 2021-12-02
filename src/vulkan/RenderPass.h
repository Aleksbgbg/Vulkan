#ifndef VULKAN_SRC_VULKAN_RENDERPASS_H_
#define VULKAN_SRC_VULKAN_RENDERPASS_H_

#include <vulkan/vulkan.h>

#include "structures/RenderPassCreateInfo.h"

class RenderPass {
  friend class Swapchain;
  friend class Pipeline;
  friend class CommandBuffer;
  friend class ImGuiInstance;

 public:
  RenderPass();
  RenderPass(VkDevice device, const RenderPassCreateInfoBuilder& infoBuilder);

  RenderPass(const RenderPass&) = delete;
  RenderPass(RenderPass&& other) noexcept;

  ~RenderPass();

  RenderPass& operator=(const RenderPass&) = delete;
  RenderPass& operator=(RenderPass&& other) noexcept;

 private:
  VkDevice device;
  VkRenderPass renderPass;
};

#endif  // VULKAN_SRC_VULKAN_RENDERPASS_H_
