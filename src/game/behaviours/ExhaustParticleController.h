#ifndef VULKAN_SRC_GAME_BEHAVIOURS_EXHAUSTPARTICLECONTROLLER_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_EXHAUSTPARTICLECONTROLLER_H_

#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/Transform.h"
#include "game/actor/property/Visibility.h"

class ExhaustParticleController : public Behaviour {
 public:
  ExhaustParticleController(Transform& parentTransform, Visibility& visibility);

  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& parentTransform_;
  Visibility& visibility_;
  glm::vec3 previousParentPosition_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_EXHAUSTPARTICLECONTROLLER_H_
