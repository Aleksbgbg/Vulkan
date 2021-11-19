#ifndef VULKAN_SRC_GAME_RENDERS_ACTORDESCRIPTORALLOCATOR_H
#define VULKAN_SRC_GAME_RENDERS_ACTORDESCRIPTORALLOCATOR_H

#include "vulkan/DescriptorSet.h"

class ActorDescriptorAllocator {
 public:
  virtual ~ActorDescriptorAllocator() = default;

  virtual DescriptorSet AllocateDescriptor() const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_ACTORDESCRIPTORALLOCATOR_H
