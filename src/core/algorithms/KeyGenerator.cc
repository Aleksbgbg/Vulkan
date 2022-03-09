#include "KeyGenerator.h"

#include <chrono>
#include <thread>

#include "core/math/bits.h"

namespace {

constexpr std::size_t SetBitRange(const std::size_t value,
                                  const std::size_t bits,
                                  const std::size_t position) {
  return (value & bits) << position;
}

constexpr std::size_t MAX_BITS = sizeof(Key) * 8;

constexpr std::size_t TIMESTAMP_SIZE = 42;
constexpr std::size_t TIMESTAMP_MAX = ValueOfBits(TIMESTAMP_SIZE);
constexpr std::size_t TIMESTAMP_SHIFT = MAX_BITS - TIMESTAMP_SIZE;

constexpr std::size_t THREAD_ID_SIZE = 10;
constexpr std::size_t THREAD_ID_MAX = ValueOfBits(THREAD_ID_SIZE);
constexpr std::size_t THREAD_ID_SHIFT = TIMESTAMP_SHIFT - THREAD_ID_SIZE;

constexpr std::size_t SEQUENCE_ID_SIZE = 12;
constexpr std::size_t SEQUENCE_ID_MAX = ValueOfBits(SEQUENCE_ID_SIZE);
constexpr std::size_t SEQUENCE_ID_SHIFT = THREAD_ID_SHIFT - SEQUENCE_ID_SIZE;

}  // namespace

KeyGenerator::KeyGenerator() : sequence_(0) {}

Key KeyGenerator::CreateKey() {
  const std::size_t time =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count() %
      TIMESTAMP_MAX;
  const std::size_t threadId =
      std::hash<std::thread::id>()(std::this_thread::get_id()) % THREAD_ID_MAX;
  const std::size_t sequence = (++sequence_) % SEQUENCE_ID_MAX;

  return SetBitRange(time, TIMESTAMP_MAX, TIMESTAMP_SHIFT) |
         SetBitRange(threadId, THREAD_ID_MAX, THREAD_ID_SHIFT) |
         SetBitRange(sequence, SEQUENCE_ID_MAX, SEQUENCE_ID_SHIFT);
}
