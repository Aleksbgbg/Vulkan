#include "ParticleComputeHandler.h"

#include "renderer/vulkan/buffer_structures/ParticleSpawnParams.h"

ParticleComputeHandler::ParticleComputeHandler(
    RandomNumberGenerator& randomNumberGenerator,
    const SpawnControllable& spawnControllable,
    const Transformable& transformable, const glm::vec3 spawnRegionLow,
    const glm::vec3 spawnRegionHigh)
    : randomNumberGenerator_(randomNumberGenerator),
      spawnControllable_(spawnControllable),
      transformable_(transformable),
      spawnRegionLow_(spawnRegionLow),
      spawnRegionHigh_(spawnRegionHigh) {}

void ParticleComputeHandler::Fill(
    const std::unordered_map<u32, Descriptor>& descriptors) const {
  const ParticleSpawnParams spawnParams{
      .randomSeed = randomNumberGenerator_.RandomUint(0, UINT32_MAX),
      .enableRespawn = spawnControllable_.GetEnableSpawn(),
      .baseTransform = transformable_.GetTransform(),
      .spawnRegionLow = spawnRegionLow_,
      .spawnRegionHigh = spawnRegionHigh_};
  descriptors.at(0).buffer.MapCopy(&spawnParams);
}
