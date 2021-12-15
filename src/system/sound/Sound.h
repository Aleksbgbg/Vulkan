#ifndef VULKAN_SRC_SYSTEM_SOUND_SOUND_H_
#define VULKAN_SRC_SYSTEM_SOUND_SOUND_H_

#include <string_view>

namespace sys {

class Sound {
 public:
  void LoopSound(const std::string_view filename);
};

}  // namespace sys

#endif  // VULKAN_SRC_SYSTEM_SOUND_SOUND_H_
