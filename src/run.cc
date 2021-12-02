#include "run.h"

#include <stdexcept>

#include "App.h"
#include "general/files/file.h"
#include "general/logging/log.h"

int RunVulkanApp(const sys::System& system) {
  const std::unique_ptr<sys::Window> window = system.SpawnWindow(1920, 1080);
  return App(*window, std::make_unique<Vulkan>(system, *window)).Run();
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
