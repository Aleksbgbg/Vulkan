#ifndef SRC_ENGINE_PROPERTY_TYPE_SOUNDEMITTER_H_
#define SRC_ENGINE_PROPERTY_TYPE_SOUNDEMITTER_H_

#include "engine/property/Property.h"
#include "engine/property/PropertyKey.h"
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

#endif  // SRC_ENGINE_PROPERTY_TYPE_SOUNDEMITTER_H_
