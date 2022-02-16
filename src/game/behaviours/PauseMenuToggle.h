#ifndef VULKAN_SRC_GAME_BEHAVIOURS_PAUSEMENUTOGGLE_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_PAUSEMENUTOGGLE_H_

#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/GraphicalInterface.h"

class PauseMenuToggle : public Behaviour {
 public:
  PauseMenuToggle(GraphicalInterface& interface);

  void UpdateModel(const UpdateContext& context) override;

 private:
  GraphicalInterface& interface_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_PAUSEMENUTOGGLE_H_
