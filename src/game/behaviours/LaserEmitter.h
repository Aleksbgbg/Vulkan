#ifndef VULKAN_SRC_GAME_BEHAVIOURS_LASEREMITTER_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_LASEREMITTER_H_

#include <game/actor/property/Transform.h>

#include "game/actor/behaviour/Behaviour.h"
#include "game/composition/MeshHandle.h"
#include "game/composition/SceneComposer.h"

class LaserEmitter : public Behaviour {
 public:
  LaserEmitter(Transform& parentTransform, SceneComposer& scene,
               MeshHandle laserMesh);

  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& parentTransform_;
  SceneComposer& scene_;
  MeshHandle laserMesh_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_LASEREMITTER_H_
