#ifndef VULKAN_SRC_GAME_RENDERS_PARTICLECONTROLLER_H
#define VULKAN_SRC_GAME_RENDERS_PARTICLECONTROLLER_H

#include <memory>

#include "ParticleStream.h"

class ParticleController {
 public:
  virtual ~ParticleController() = default;

  virtual ParticleStream& CreateParticleStream() = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_PARTICLECONTROLLER_H
