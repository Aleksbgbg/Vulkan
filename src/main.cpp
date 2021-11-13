#include <stdexcept>

#include "App.h"
#include "general/files/file.h"
#include "general/logging/log.h"
#include "general/windowing/system_window.h"

int RunVulkanApp() {
  return App(InitializeSystemWindow(1920, 1080)).Run();
}

int run() {
  try {
    return RunVulkanApp();
  } catch (const std::exception& exception) {
    const std::string errorString =
        std::string("Error during run:\n") + exception.what();
    ImmediateLog(errorString);
    file::WriteFile("RunDebug.txt", errorString);
    throw;
  }
}

int main() {
  return run();
}
