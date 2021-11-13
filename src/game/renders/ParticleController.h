#ifndef VULKAN_SRC_GAME_RENDERS_PARTICLECONTROLLER_H
#define VULKAN_SRC_GAME_RENDERS_PARTICLECONTROLLER_H

#include "general/geometry/Rect.h"
#include "util/include/glm.h"

class ParticleController {
 public:
  virtual ~ParticleController() = default;

  virtual void SetSpawnArea(const Rectf& spawnArea) = 0;
  virtual void SetTransform(const glm::mat4& transform) = 0;
  virtual void SetEnabled(const bool enabled) = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERS_PARTICLECONTROLLER_H
