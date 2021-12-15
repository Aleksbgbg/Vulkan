#include "System.h"

namespace sys {

System::System() : sound_() {}

std::unique_ptr<Window> System::SpawnWindow(const u32 width,
                                            const u32 height) const {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);

  const Recti windowRect = Recti::FromRegion(50, 50, width, height);

  SDL_Window* window = SDL_CreateWindow(
      "Vulkan", windowRect.X(), windowRect.Y(), windowRect.Width(),
      windowRect.Height(),
      SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  return CreateWindow(window, windowRect);
}

Sound& System::GetSound() {
  return sound_;
}

}  // namespace sys
