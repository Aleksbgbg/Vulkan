#ifndef VULKAN_SRC_GAME_NETWORK_H
#define VULKAN_SRC_GAME_NETWORK_H

#include "util/types.h"

class Network {
 public:
  enum class Attribute {
    RotationX,
    RotationY,
    VelocityZ,
    PositionX,
    PositionY,
    PositionZ,
  };

  virtual ~Network() = default;

  virtual float ReadFloat(const u32 actorId,
                          const Attribute attribute) const = 0;
};

#endif  // VULKAN_SRC_GAME_NETWORK_H
