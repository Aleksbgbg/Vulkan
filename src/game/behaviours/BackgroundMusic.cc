#include "BackgroundMusic.h"

#include "util/filenames.h"

BackgroundMusic::BackgroundMusic(SoundEmitter& soundEmitter)
    : soundEmitter_(soundEmitter) {}

void BackgroundMusic::OnSpawn() {
  soundEmitter_.LoopSound(BACKGROUND_MUSIC_FILENAME);
}
