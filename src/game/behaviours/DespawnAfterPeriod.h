#ifndef SRC_GAME_BEHAVIOURS_DESPAWNAFTERPERIOD_H_
#define SRC_GAME_BEHAVIOURS_DESPAWNAFTERPERIOD_H_

#include "engine/actor/Actor.h"
#include "engine/behaviour/Behaviour.h"

class DespawnAfterPeriod : public Behaviour {
 public:
  DespawnAfterPeriod(game::Actor& actor, float period);

  void UpdateModel(const UpdateContext& context) override;

 private:
  game::Actor& actor_;
  float timeLeftToLive_;
};

#endif  // SRC_GAME_BEHAVIOURS_DESPAWNAFTERPERIOD_H_
