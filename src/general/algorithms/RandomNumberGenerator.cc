#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator()
    : generator(std::random_device()()) {}

i32 RandomNumberGenerator::RandomInt(const i32 min, const i32 max) {
  return std::uniform_int_distribution<i32>(min, max)(generator);
}

u32 RandomNumberGenerator::RandomUint(const u32 min, const u32 max) {
  return std::uniform_int_distribution<u32>(min, max)(generator);
}

float RandomNumberGenerator::RandomFloat(const float min, const float max) {
  return std::uniform_real_distribution<float>(min, max)(generator);
}
