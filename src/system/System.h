#ifndef VULKAN_SRC_SYSTEM_SYSTEM_H_
#define VULKAN_SRC_SYSTEM_SYSTEM_H_

#include <memory>

#include "VulkanSystem.h"
#include "util/include/sdl.h"
#include "windowing/Window.h"

namespace sys {

class System : public VulkanSystem {
 public:
  virtual ~System() = default;

  std::unique_ptr<Window> SpawnWindow(const u32 width, const u32 height) const;

 private:
  virtual std::unique_ptr<Window> CreateWindow(
      SDL_Window* const window, const Recti windowRect) const = 0;
};

}  // namespace sys

#endif  // VULKAN_SRC_SYSTEM_SYSTEM_H_
