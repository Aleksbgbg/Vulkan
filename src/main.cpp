#include <fstream>
#include <iostream>
#include <stdexcept>

#include "App.h"

void WriteFile(const char* const filename, std::string content) {
  std::ofstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error(std::string("Failed to open file '") + filename +
                             "'.");
  }

  file.write(content.c_str(), content.size());
}

int run() {
  try {
    return App().Run();
  } catch (const std::exception& exception) {
    const std::string errorString =
        std::string("Error during run:\n") + exception.what();
    std::cout << errorString << std::endl;
    WriteFile("RunDebug.txt", errorString);
    throw;
  }
}

int main() {
  return run();
}
