#include <exception>
#include <iostream>

#include "Server.h"

int main() {
  try {
    Server().Run();
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
    throw;
  }
  return 0;
}
