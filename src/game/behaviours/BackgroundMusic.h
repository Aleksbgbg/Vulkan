#ifndef SRC_GAME_BEHAVIOURS_BACKGROUNDMUSIC_H_
#define SRC_GAME_BEHAVIOURS_BACKGROUNDMUSIC_H_

#include "engine/behaviour/Behaviour.h"
#include "engine/property/type/SoundEmitter.h"

class BackgroundMusic : public Behaviour {
 public:
  BackgroundMusic(SoundEmitter& soundEmitter, SoundHandle sound);

  void OnSpawn() override;

 private:
  SoundEmitter& soundEmitter_;
  SoundHandle sound_;
};

#endif  // SRC_GAME_BEHAVIOURS_BACKGROUNDMUSIC_H_
