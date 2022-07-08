#ifndef SRC_GAME_BEHAVIOURS_LASEREMITTER_H_
#define SRC_GAME_BEHAVIOURS_LASEREMITTER_H_

#include "engine/behaviour/Behaviour.h"
#include "engine/composition/CompositionBuilder.h"
#include "engine/composition/parameter_utils.h"
#include "engine/property/type/SoundEmitter.h"
#include "engine/property/type/Transform.h"

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

#endif  // SRC_GAME_BEHAVIOURS_LASEREMITTER_H_
