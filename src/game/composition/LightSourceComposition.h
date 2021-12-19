#ifndef VULKAN_SRC_GAME_COMPOSITION_LIGHTSOURCECOMPOSITION_H_
#define VULKAN_SRC_GAME_COMPOSITION_LIGHTSOURCECOMPOSITION_H_

#include "game/LightSource.h"

class LightSourceComposition {
 public:
  virtual ~LightSourceComposition() = default;

  virtual LightSource GetLightSource() const = 0;
};

#endif  // VULKAN_SRC_GAME_COMPOSITION_LIGHTSOURCECOMPOSITION_H_
