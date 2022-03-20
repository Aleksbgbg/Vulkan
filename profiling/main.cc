#include "core/diagnostics/profile.h"
#include "core/files/png/png.h"
#include "core/types.h"

int main() {
  for (u32 iteration = 0; iteration < 10; ++iteration) {
    PROFILING_BEGIN("Profiling")
    const Bitmap nebula = file::ReadPng("../src/resources/Nebula.png");
    const Bitmap sun = file::ReadPng("../src/resources/Sun.png");
    const Bitmap spaceship =
        file::ReadPng("../src/resources/InterstellarRunner.png");
    PROFILING_END
  }

  return 0;
}
