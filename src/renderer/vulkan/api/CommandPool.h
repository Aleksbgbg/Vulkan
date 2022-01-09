#ifndef VULKAN_SRC_RENDERER_VULKAN_API_COMMANDPOOL_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_COMMANDPOOL_H_

#include <vulkan/vulkan.h>

#include "CommandBuffer.h"
#include "renderer/vulkan/api/structures/CommandPoolCreateInfo.h"

namespace vk {

class CommandPool {
 public:
  CommandPool();
  CommandPool(VkDevice device, VkQueue queue,
              const CommandPoolCreateInfoBuilder& infoBuilder);

  CommandPool(const CommandPool&) = delete;
  CommandPool(CommandPool&& other) noexcept;

  ~CommandPool();

  CommandPool& operator=(const CommandPool&) = delete;
  CommandPool& operator=(CommandPool&& other) noexcept;

  CommandBuffer AllocatePrimaryCommandBuffer() const;
  CommandBuffer AllocateSecondaryCommandBuffer() const;

 private:
  CommandBuffer AllocateCommandBuffer(const VkCommandBufferLevel level) const;

 private:
  VkDevice device;
  VkQueue queue;
  VkCommandPool commandPool;
};

}  // namespace vk

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_COMMANDPOOL_H_
