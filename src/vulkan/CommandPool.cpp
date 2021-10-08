#include "CommandPool.h"

#include "error.h"

CommandPool::CommandPool(VkDevice device, VkQueue queue,
                         CommandPoolCreateInfoBuilder& infoBuilder)
    : device(device), queue(queue) {
  PROCEED_ON_VALID_RESULT(
      vkCreateCommandPool(device, infoBuilder.Build(), nullptr, &commandPool));
}

CommandPool::~CommandPool() {
  if (commandPool != nullptr) {
    vkDestroyCommandPool(device, commandPool, nullptr);
  }
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
