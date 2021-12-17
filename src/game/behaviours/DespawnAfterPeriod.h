#ifndef VULKAN_SRC_GAME_BEHAVIOURS_DESPAWNAFTERPERIOD_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_DESPAWNAFTERPERIOD_H_

#include "game/actor/Actor.h"
#include "game/actor/behaviour/Behaviour.h"

class DespawnAfterPeriod : public Behaviour {
 public:
  DespawnAfterPeriod(game::Actor& actor, float period);

  void UpdateModel(const UpdateContext& context) override;

 private:
  game::Actor& actor_;
  float timeLeftToLive_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_DESPAWNAFTERPERIOD_H_
