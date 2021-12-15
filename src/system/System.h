#ifndef VULKAN_SRC_SYSTEM_SYSTEM_H_
#define VULKAN_SRC_SYSTEM_SYSTEM_H_

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
  Sound& GetSound();

 private:
  virtual std::unique_ptr<Window> CreateWindow(
      SDL_Window* const window, const Recti windowRect) const = 0;

 private:
  Sound sound_;
};

}  // namespace sys

#endif  // VULKAN_SRC_SYSTEM_SYSTEM_H_
