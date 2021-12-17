#include "DespawnAfterPeriod.h"

DespawnAfterPeriod::DespawnAfterPeriod(game::Actor& actor, float period)
    : actor_(actor), timeLeftToLive_(period) {}

void DespawnAfterPeriod::UpdateModel(const UpdateContext& context) {
  timeLeftToLive_ -= context.deltaTime;

  if (timeLeftToLive_ <= 0.0f) {
    actor_.Despawn();
  }
}
