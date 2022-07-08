#ifndef SRC_CORE_ALGORITHMS_KEYGENERATOR_H_
#define SRC_CORE_ALGORITHMS_KEYGENERATOR_H_

#include "core/types.h"

using Key = u64;

class KeyGenerator {
 public:
  KeyGenerator();

  Key CreateKey();

 private:
  u32 sequence_;
};

#endif  // SRC_CORE_ALGORITHMS_KEYGENERATOR_H_
