#include "SynchronisationPack.h"

const Semaphore* SynchronisationPack::GetWaitSemaphore() const {
  return waitSemaphore;
}

const Semaphore* SynchronisationPack::GetSignalSemaphore() const {
  return signalSemaphore;
}

const Fence* SynchronisationPack::GetSignalFence() const {
  return signalFence;
}

SynchronisationPack& SynchronisationPack::SetWaitSemaphore(
    const Semaphore* const semaphore) {
  waitSemaphore = semaphore;
  return *this;
}

SynchronisationPack& SynchronisationPack::SetSignalSemaphore(
    const Semaphore* const semaphore) {
  signalSemaphore = semaphore;
  return *this;
}

SynchronisationPack& SynchronisationPack::SetSignalFence(
    const Fence* const fence) {
  signalFence = fence;
  return *this;
}
