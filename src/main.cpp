#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <fstream>

#include "App.h"

void WriteFile(const char* const filename, std::string content) {
  std::ofstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file.");
  }

  file.write(content.c_str(), content.size());
}

int run() {
  try {
    auto a = App();
    auto b = a.Run();
    return b;
    // return App().Run();
  } catch (const std::exception& exception) {
    std::string str = std::string() + "Error during run:\n" + exception.what() + "\nPress any key to continue...";
    std::cout << str << std::endl;
    WriteFile("RunDebug.txt", str);
    std::getchar();
    std::getchar();
    std::getchar();
    throw;
  }
}

int main() {
  auto r = run();
  int x =5;
  return r;
}
