#ifndef VULKAN_SRC_GAME_ACTOR_PROPERTY_SOUNDEMITTER_H_
#define VULKAN_SRC_GAME_ACTOR_PROPERTY_SOUNDEMITTER_H_

#include "Property.h"
#include "PropertyKey.h"
#include "system/sound/Sound.h"

class SoundEmitter : public Property {
 public:
  static constexpr PropertyKey Key() {
    return PropertyKey::SoundEmitter;
  }

  SoundEmitter(sys::Sound& sound);

  void Loop(SoundHandle sound);
  void Play(SoundHandle sound);

 private:
  sys::Sound& sound_;
};

#endif  // VULKAN_SRC_GAME_ACTOR_PROPERTY_SOUNDEMITTER_H_
