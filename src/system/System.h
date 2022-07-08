#ifndef SRC_SYSTEM_SYSTEM_H_
#define SRC_SYSTEM_SYSTEM_H_

#include <memory>

#include "renderer/vulkan/VulkanSystem.h"
#include "system/sound/Sound.h"
#include "util/include/sdl.h"
#include "windowing/Window.h"

namespace sys {

class System : public VulkanSystem {
 public:
  System();

  virtual ~System() = default;

  std::unique_ptr<Window> SpawnWindow(const u32 width, const u32 height) const;

 private:
  virtual std::unique_ptr<Window> CreateWindow(
      SDL_Window* const window, const Recti windowRect) const = 0;
};

}  // namespace sys

#endif  // SRC_SYSTEM_SYSTEM_H_
