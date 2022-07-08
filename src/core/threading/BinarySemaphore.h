#ifndef SRC_CORE_THREADING_BINARYSEMAPHORE_H_
#define SRC_CORE_THREADING_BINARYSEMAPHORE_H_

#include <condition_variable>
#include <mutex>

class BinarySemaphore {
 public:
  void Acquire();
  void Release();

 private:
  std::mutex mutex_;
  std::condition_variable condition_;
};

#endif  // SRC_CORE_THREADING_BINARYSEMAPHORE_H_
