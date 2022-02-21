#ifndef VULKAN_SRC_GAME_BEHAVIOURS_EXHAUSTPARTICLECONTROLLER_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_EXHAUSTPARTICLECONTROLLER_H_

#include "engine/behaviour/Behaviour.h"
#include "engine/property/type/Transform.h"
#include "engine/property/type/Visibility.h"

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
