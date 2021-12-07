#ifndef VULKAN_SRC_GAME_SPAWNCONTROLLABLE_H_
#define VULKAN_SRC_GAME_SPAWNCONTROLLABLE_H_

class SpawnControllable {
 public:
  virtual ~SpawnControllable() = default;

  virtual bool GetEnableSpawn() const = 0;
};

#endif  // VULKAN_SRC_GAME_SPAWNCONTROLLABLE_H_
