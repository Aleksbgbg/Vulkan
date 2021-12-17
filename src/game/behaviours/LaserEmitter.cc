#include "LaserEmitter.h"

#include "ConstantMovement.h"
#include "DespawnAfterPeriod.h"
#include "game/composition/CompositionBuilder.h"
#include "game/composition/behaviour_utils.h"

LaserEmitter::LaserEmitter(const ParameterPack& parameters)
    : parentTransform_(parameters.GetParentTransform()),
      scene_(parameters.GetScene()),
      laserMesh_(parameters.GetLaserMesh()) {}

void LaserEmitter::UpdateModel(const UpdateContext& context) {
  if (context.controls.IsControlActive(Control::Shoot)) {
    scene_.Actor()
        .Attach(BEHAVIOUR(
            ConstantMovement,
            ConstantMovement::ParameterPack()
                .SetTransform(actor.RetrieveProperty<Transform>()
                                  .Move(parentTransform_.GetPosition())
                                  .Scale(parentTransform_.GetScale())
                                  .Rotate(parentTransform_.GetRotation()))
                .SetForwardVelocity(40.0f)))
        .Attach(BEHAVIOUR(DespawnAfterPeriod, actor, 5.0f))
        .Mesh(laserMesh_)
        .Spawn();
  }
}
