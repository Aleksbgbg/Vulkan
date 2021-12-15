#include "run.h"

#include <stdexcept>

#include "App.h"
#include "general/files/file.h"
#include "general/logging/log.h"
#include "util/build_definition.h"

int RunVulkanApp(sys::System& system) {
#if defined(PROFILING)
  const auto startTime = std::chrono::high_resolution_clock::now();
#endif

  const std::unique_ptr<sys::Window> window = system.SpawnWindow(1920, 1080);
  sys::Sound& sound = system.GetSound();

  Vulkan vulkan(system, *window);
  App app(*window, sound, vulkan);

#if defined(PROFILING)
  const auto endTime = std::chrono::high_resolution_clock::now();
  const float startupDuration =
      std::chrono::duration<float, std::chrono::milliseconds::period>(endTime -
                                                                      startTime)
          .count();
  ImmediateLog(std::string("Startup took ") + std::to_string(startupDuration) +
               "ms");
#endif

  return app.Run();
}

int run(sys::System& system) {
  try {
    return RunVulkanApp(system);
  } catch (const std::exception& exception) {
    const std::string errorString =
        std::string("Error during run:\n") + exception.what();
    ImmediateLog(errorString);
    file::WriteFile("RunDebug.txt", errorString);
    throw;
  }
}
