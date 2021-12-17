#include "LaserEmitter.h"

#include "ConstantMovement.h"
#include "DespawnAfterPeriod.h"
#include "game/composition/CompositionBuilder.h"

LaserEmitter::LaserEmitter(Transform& parentTransform, SceneComposer& scene,
                           MeshHandle laserMesh)
    : parentTransform_(parentTransform), scene_(scene), laserMesh_(laserMesh) {}

void LaserEmitter::UpdateModel(const UpdateContext& context) {
  if (context.controls.IsControlActive(Control::Shoot)) {
    scene_.Actor()
        .Attach(BEHAVIOUR(ConstantMovement,
                          actor.RetrieveProperty<Transform>()
                              .Move(parentTransform_.GetPosition())
                              .Scale(parentTransform_.GetScale())
                              .Rotate(parentTransform_.GetRotation()),
                          glm::toMat3(parentTransform_.GetRotation()) *
                              glm::vec3(0.0f, 0.0f, 40.0f)))
        .Attach(BEHAVIOUR(DespawnAfterPeriod, actor, 5.0f))
        .Mesh(laserMesh_)
        .Spawn();
  }
}
