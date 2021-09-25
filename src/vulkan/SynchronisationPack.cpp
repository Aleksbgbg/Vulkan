#include "SynchronisationPack.h"

Semaphore* SynchronisationPack::GetWaitSemaphore() const {
  return waitSemaphore;
}

Semaphore* SynchronisationPack::GetSignalSemaphore() const {
  return signalSemaphore;
}

Fence* SynchronisationPack::GetSignalFence() const {
  return signalFence;
}

SynchronisationPack& SynchronisationPack::SetWaitSemaphore(Semaphore* const semaphore) {
  waitSemaphore = semaphore;
  return *this;
}

SynchronisationPack& SynchronisationPack::SetSignalSemaphore(Semaphore* const semaphore) {
  signalSemaphore = semaphore;
  return *this;
}

SynchronisationPack& SynchronisationPack::SetSignalFence(Fence* const fence) {
  signalFence = fence;
  return *this;
}


