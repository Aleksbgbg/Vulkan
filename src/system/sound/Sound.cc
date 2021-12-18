#include "Sound.h"

#include <cmath>
#include <stdexcept>

namespace {

using SoundType = float;

}

namespace sys {

Sound::Sound() : audioDeviceId_(0), value_(0) {}

Sound::~Sound() {
  SDL_CloseAudioDevice(audioDeviceId_);
}

void Sound::LoopSound(const std::string_view filename) {
  SDL_AudioSpec desired{};
  desired.freq = 48000;
  desired.format = AUDIO_F32SYS;
  desired.channels = 1;
  desired.samples = sizeof(SoundType) * desired.channels * 2048;
  desired.callback = Sound::Callback;
  desired.userdata = this;

  SDL_AudioSpec obtained;

  audioDeviceId_ = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);

  if (audioDeviceId_ == 0) {
    throw std::runtime_error(std::string("Error in opening audio device: ") +
                             SDL_GetError());
  }

  SDL_PauseAudioDevice(audioDeviceId_, 0);
}

void Sound::Callback(void* const userData, u8* const stream, const i32 length) {
  reinterpret_cast<Sound*>(userData)->InstanceCallback(stream, length);
}

void Sound::InstanceCallback(u8* streamBytes, const i32 length) {
  constexpr float pi = 3.14159265359f;

  SoundType* stream = reinterpret_cast<SoundType*>(streamBytes);

  static float frequency = 10.0f;
  frequency += 10.0f;

  for (u32 index = 0; index < (length / sizeof(SoundType)); ++index) {
    stream[index] =
        std::sin(2.0f * pi * (value_ + index) * (1.0f / 48000.0f) * frequency);
  }

  value_ += length;
}

}  // namespace sys
