#include "core/diagnostics/profile.h"
#include "core/files/file.h"
#include "core/files/png/png.h"
#include "core/types.h"

int main() {
  for (u32 iteration = 0; iteration < 10; ++iteration) {
    PROFILING_BEGIN("Profiling")
    const Bitmap nebula =
        file::ReadPng(file::ReadFile("../src/resources/Nebula.png"));
    const Bitmap sun =
        file::ReadPng(file::ReadFile("../src/resources/Sun.png"));
    const Bitmap spaceship = file::ReadPng(
        file::ReadFile("../src/resources/InterstellarRunner.png"));
    PROFILING_END
  }

  return 0;
}
