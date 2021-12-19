#ifndef VULKAN_SRC_GAME_BEHAVIOURS_LASEREMITTER_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_LASEREMITTER_H_

#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/SoundEmitter.h"
#include "game/actor/property/Transform.h"
#include "game/composition/CompositionBuilder.h"
#include "game/composition/parameter_utils.h"

class LaserEmitter : public Behaviour {
 public:
  PARAMETER_PACK_BEGIN
  PARAMETER_TYPE(Transform, ParentTransform)
  PARAMETER_TYPE(CompositionBuilder, LaserComposition)
  PARAMETER_TYPE(SoundEmitter, SoundEmitter)
  PARAMETER_VALUE(SoundHandle, SoundEffect)
  PARAMETER_PACK_END

  LaserEmitter(const ParameterPack& parameters);

  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& parentTransform_;
  CompositionBuilder laserComposition_;
  SoundEmitter& soundEmitter_;
  SoundHandle soundEffect_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_LASEREMITTER_H_
