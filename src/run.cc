#include "run.h"

#include <stdexcept>

#include "App.h"
#include "general/files/file.h"
#include "general/logging/log.h"
#include "util/build_definition.h"

int RunVulkanApp(const sys::System& system) {
  const std::unique_ptr<sys::Window> window = system.SpawnWindow(1920, 1080);

#if defined(PROFILING)
  const auto startTime = std::chrono::high_resolution_clock::now();
#endif
  App app(*window, std::make_unique<Vulkan>(system, *window));
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

int run(const sys::System& system) {
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
