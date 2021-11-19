#ifndef VULKAN_SRC_GAME_RENDERS_ACTORDESCRIPTORBINDER_H
#define VULKAN_SRC_GAME_RENDERS_ACTORDESCRIPTORBINDER_H

#include "util/types.h"
#include "vulkan/CommandBuffer.h"

class ActorDescriptorBinder {
 public:
  virtual ~ActorDescriptorBinder() = default;

  virtual void AddDescriptor(DescriptorSet descriptorSet) = 0;

  virtual void BindActorDescriptorSet(const PipelineLayout& pipelineLayout,
                                      const CommandBuffer& commandBuffer,
                                      const u32 actorIndex) const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_ACTORDESCRIPTORBINDER_H
