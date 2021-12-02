#ifndef VULKAN_SRC_GAME_RENDERS_PARTICLECONTROLLER_H_
#define VULKAN_SRC_GAME_RENDERS_PARTICLECONTROLLER_H_

#include <memory>

#include "ParticleStream.h"

class ParticleController {
 public:
  virtual ~ParticleController() = default;

  virtual ParticleStream& CreateParticleStream() = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_PARTICLECONTROLLER_H_
