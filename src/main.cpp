#include <stdexcept>

#include "App.h"
#include "general/files/file.h"
#include "general/logging/log.h"

int run() {
  try {
    return App().Run();
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
