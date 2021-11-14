#include "CommandPool.h"

#include "error.h"

CommandPool::CommandPool() : commandPool(nullptr) {}

CommandPool::CommandPool(VkDevice device, VkQueue queue,
                         const CommandPoolCreateInfoBuilder& infoBuilder)
    : device(device), queue(queue) {
  PROCEED_ON_VALID_RESULT(
      vkCreateCommandPool(device, infoBuilder.Build(), nullptr, &commandPool));
}

CommandPool::CommandPool(CommandPool&& other) noexcept
    : device(other.device), queue(other.queue), commandPool(other.commandPool) {
  other.commandPool = nullptr;
}

CommandPool::~CommandPool() {
  if (commandPool != nullptr) {
    vkDestroyCommandPool(device, commandPool, nullptr);
  }
}

CommandPool& CommandPool::operator=(CommandPool&& other) noexcept {
  std::swap(device, other.device);
  queue = other.queue;
  std::swap(commandPool, other.commandPool);
  return *this;
}

CommandBuffer CommandPool::AllocatePrimaryCommandBuffer() {
  return AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
}

CommandBuffer CommandPool::AllocateCommandBuffer(
    const VkCommandBufferLevel level) {
  return CommandBuffer(device, queue, commandPool,
                       CommandBufferAllocateInfoBuilder()
                           .SetCommandPool(commandPool)
                           .SetLevel(level));
}
