#ifndef VULKAN_SRC_GAME_ACTOR_PROPERTY_PARTICLECONTROLLER_H_
#define VULKAN_SRC_GAME_ACTOR_PROPERTY_PARTICLECONTROLLER_H_

#include "Property.h"
#include "PropertyKey.h"
#include "game/SpawnControllable.h"

class ParticleController : public Property, public SpawnControllable {
 public:
  static PropertyKey Key();

  ParticleController();

  bool GetEnableSpawn() const override;

  void EnableParticles(const bool enable);

 private:
  bool enableParticles_;
};

#endif  // VULKAN_SRC_GAME_ACTOR_PROPERTY_PARTICLECONTROLLER_H_
