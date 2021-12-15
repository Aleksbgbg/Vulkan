#ifndef VULKAN_SRC_GAME_ACTOR_PROPERTY_SOUNDEMITTER_H_
#define VULKAN_SRC_GAME_ACTOR_PROPERTY_SOUNDEMITTER_H_

#include <string_view>

#include "Property.h"
#include "PropertyKey.h"
#include "system/sound/Sound.h"

class SoundEmitter : public Property {
 public:
  static PropertyKey Key();

  SoundEmitter(sys::Sound& sound);

  void LoopSound(const std::string_view filename);

 private:
  sys::Sound& sound_;
};

#endif  // VULKAN_SRC_GAME_ACTOR_PROPERTY_SOUNDEMITTER_H_
