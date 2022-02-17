#include "BinarySemaphore.h"

void BinarySemaphore::Acquire() {
  std::unique_lock lock(mutex_);
  condition_.wait(lock);
}

void BinarySemaphore::Release() {
  std::lock_guard guard(mutex_);
  condition_.notify_one();
}
