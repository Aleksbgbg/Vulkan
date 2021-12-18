#include "SoundEmitter.h"

PropertyKey SoundEmitter::Key() {
  return PropertyKey::SoundEmitter;
}

SoundEmitter::SoundEmitter(sys::Sound& sound) : sound_(sound) {}

void SoundEmitter::Loop(const SoundHandle sound) {
  sound_.Loop(sound);
}

void SoundEmitter::Play(const SoundHandle sound) {
  sound_.Play(sound);
}
