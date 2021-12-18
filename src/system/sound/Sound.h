#ifndef VULKAN_SRC_SYSTEM_SOUND_SOUND_H_
#define VULKAN_SRC_SYSTEM_SOUND_SOUND_H_

#include <SDL_audio.h>

#include <string_view>

#include "util/types.h"

namespace sys {

class Sound {
 public:
  Sound();
  ~Sound();

  void LoopSound(const std::string_view filename);

 private:
  static void Callback(void* userData, u8* stream, const i32 length);
  void InstanceCallback(u8* streamBytes, const i32 length);

 private:
  SDL_AudioDeviceID audioDeviceId_;
  u32 value_;
};

}  // namespace sys

#endif  // VULKAN_SRC_SYSTEM_SOUND_SOUND_H_
