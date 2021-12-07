#ifndef VULKAN_SRC_GAME_BEHAVIOURS_EXHAUSTPARTICLECONTROLLER_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_EXHAUSTPARTICLECONTROLLER_H_

#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/ParticleController.h"
#include "game/actor/property/Transform.h"

class ExhaustParticleController : public Behaviour {
 public:
  ExhaustParticleController(Transform& parentTransform,
                            ParticleController& particleController);

  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& parentTransform_;
  ParticleController& particleController_;
  glm::vec3 previousParentPosition_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_EXHAUSTPARTICLECONTROLLER_H_
