#include "System.h"

namespace sys {

System::System() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
}

std::unique_ptr<Window> System::SpawnWindow(const u32 width,
                                            const u32 height) const {
  const Recti windowRect = Recti::FromRegion(50, 50, width, height);

  SDL_Window* window = SDL_CreateWindow(
      "Yomi", windowRect.X(), windowRect.Y(), windowRect.Width(),
      windowRect.Height(),
      SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  return CreateWindow(window, windowRect);
}

}  // namespace sys
