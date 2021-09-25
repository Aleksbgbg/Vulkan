#include <stdexcept>
#include <iostream>
#include <cstdio>

#include "App.h"

App InitApp() {
  try {
    return App();
  } catch (const std::exception& exception) {
    std::cout << "Error:\n" << exception.what() << "\nPress any key to continue..." << std::endl;
    std::getchar();
    throw;
  }
}

int run() {
  return InitApp().Run();
}

int main() {
  return run();
}
