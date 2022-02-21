#include "LaserEmitter.h"

#include "ConstantMovement.h"
#include "engine/composition/behaviour_utils.h"

LaserEmitter::LaserEmitter(const ParameterPack& parameters)
    : parentTransform_(parameters.GetParentTransform()),
      laserComposition_(parameters.GetLaserComposition()),
      soundEmitter_(parameters.GetSoundEmitter()),
      soundEffect_(parameters.GetSoundEffect()) {}

void LaserEmitter::UpdateModel(const UpdateContext& context) {
  if (context.controls.IsControlActive(Control::Shoot)) {
    laserComposition_.Copy()
        .Attach(BEHAVIOUR(
            ConstantMovement,
            ConstantMovement::ParameterPack()
                .SetTransform(actor.RetrieveProperty<Transform>()
                                  .Move(parentTransform_.GetPosition())
                                  .Scale(parentTransform_.GetScale())
                                  .Rotate(parentTransform_.GetRotation()))
                .SetForwardVelocity(40.0f)))
        .Spawn();

    soundEmitter_.Play(soundEffect_);
  }
}
