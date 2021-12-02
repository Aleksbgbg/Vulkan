#ifndef VULKAN_SRC_GAME_RENDERS_ACTORDESCRIPTORALLOCATOR_H_
#define VULKAN_SRC_GAME_RENDERS_ACTORDESCRIPTORALLOCATOR_H_

#include "vulkan/DescriptorSet.h"

class ActorDescriptorAllocator {
 public:
  virtual ~ActorDescriptorAllocator() = default;

  virtual DescriptorSet AllocateDescriptor() const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_ACTORDESCRIPTORALLOCATOR_H_
