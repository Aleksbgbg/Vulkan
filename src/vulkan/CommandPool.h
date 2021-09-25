#ifndef VULKAN_SRC_VULKAN_COMMANDPOOL_H
#define VULKAN_SRC_VULKAN_COMMANDPOOL_H

#include <vulkan/vulkan.h>

#include "lifetime_semantics.h"
#include "structures/CommandPoolCreateInfo.h"
#include "CommandBuffer.h"

class CommandPool {
public:
  CommandPool() = default;
  CommandPool(VkDevice device, VkQueue queue, CommandPoolCreateInfoBuilder& infoBuilder);

  CommandPool(const CommandPool&) = delete;
  CommandPool(CommandPool&& other) noexcept : device(other.device), queue(other.queue) {
    other.commandPool = nullptr;
  }

  ~CommandPool();

  CommandPool& operator=(const CommandPool&) = delete;
  CommandPool& operator=(CommandPool&& other) noexcept {
    device = other.device;
    queue = other.queue;
    std::swap(commandPool, other.commandPool);
    return *this;
  }

  CommandBuffer AllocatePrimaryCommandBuffer();
  CommandBuffer AllocateCommandBuffer(const VkCommandBufferLevel level);

private:
  VkDevice device;
  VkQueue queue;
  VkCommandPool commandPool = nullptr;
};

#endif // VULKAN_SRC_VULKAN_COMMANDPOOL_H
