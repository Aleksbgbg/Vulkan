#include "Alignment.h"

u32 Align(const u32 address, const u32 alignment) {
  const u32 currentOffset = address % alignment;
  const u32 requiredPadding = (alignment - currentOffset) % alignment;
  return address + requiredPadding;
}
