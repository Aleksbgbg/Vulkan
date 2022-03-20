#include "log.h"

#include <iostream>

void BufferedLog(const std::string_view text) {
  std::cout << text << '\n';
}

void ImmediateLog(const std::string_view text) {
  std::cout << text << std::endl;
}
