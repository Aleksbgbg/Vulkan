#ifndef VULKAN_SRC_VULKAN_COMMANDPOOL_H_
#define VULKAN_SRC_VULKAN_COMMANDPOOL_H_

#include <vulkan/vulkan.h>

#include "CommandBuffer.h"
#include "structures/CommandPoolCreateInfo.h"

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
  CommandBuffer AllocateCommandBuffer(const VkCommandBufferLevel level) const;

 private:
  VkDevice device;
  VkQueue queue;
  VkCommandPool commandPool;
};

#endif  // VULKAN_SRC_VULKAN_COMMANDPOOL_H_
