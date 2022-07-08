#ifndef SRC_RENDERER_VULKAN_API_RENDERPASS_H_
#define SRC_RENDERER_VULKAN_API_RENDERPASS_H_

#include <vulkan/vulkan.h>

#include "renderer/vulkan/api/structures/RenderPassCreateInfo.h"

namespace vk {

class RenderPass {
  friend class Swapchain;
  friend class GraphicsPipeline;
  friend class CommandBuffer;

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

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_RENDERPASS_H_
