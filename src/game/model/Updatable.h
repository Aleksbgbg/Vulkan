#ifndef VULKAN_SRC_GAME_MODEL_UPDATABLE_H_
#define VULKAN_SRC_GAME_MODEL_UPDATABLE_H_

#include "game/model/UpdateContext.h"

class Updatable {
 public:
  virtual ~Updatable() = default;

  virtual void UpdateModel(const UpdateContext& context) = 0;
};

#endif  // VULKAN_SRC_GAME_MODEL_UPDATABLE_H_
