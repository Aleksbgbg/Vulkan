#include "ParticleController.h"

PropertyKey ParticleController::Key() {
  return PropertyKey::ParticleController;
}

ParticleController::ParticleController() : enableParticles_(false) {}

bool ParticleController::GetEnableSpawn() const {
  return enableParticles_;
}

void ParticleController::EnableParticles(const bool enable) {
  enableParticles_ = enable;
}
