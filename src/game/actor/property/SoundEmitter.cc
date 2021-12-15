#include "SoundEmitter.h"

PropertyKey SoundEmitter::Key() {
  return PropertyKey::SoundEmitter;
}

SoundEmitter::SoundEmitter(sys::Sound& sound) : sound_(sound) {}

void SoundEmitter::LoopSound(const std::string_view filename) {
  sound_.LoopSound(filename);
}
