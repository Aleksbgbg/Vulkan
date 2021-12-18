#include "BackgroundMusic.h"

BackgroundMusic::BackgroundMusic(SoundEmitter& soundEmitter, SoundHandle sound)
    : soundEmitter_(soundEmitter), sound_(sound) {}

void BackgroundMusic::OnSpawn() {
  soundEmitter_.Loop(sound_);
}
