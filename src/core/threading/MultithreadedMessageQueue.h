#ifndef VULKAN_SRC_CORE_THREADING_MULTITHREADEDMESSAGEQUEUE_H_
#define VULKAN_SRC_CORE_THREADING_MULTITHREADEDMESSAGEQUEUE_H_

#include <queue>
#include <unordered_map>

#include "BinarySemaphore.h"

template <typename TMessage>
class MultithreadedMessageQueue {
 public:
  MultithreadedMessageQueue() : messageQueue(), messageAwaiters() {}

  void PostMessage(TMessage message) {
    if (messageAwaiters.contains(message)) {
      messageAwaiters.at(message)->Release();
    } else {
      messageQueue.emplace(message);
    }
  }

  bool HasMessage() const {
    return messageQueue.size() > 0;
  }
  TMessage PopMessage() {
    TMessage message = messageQueue.front();
    messageQueue.pop();
    return message;
  }
  void WaitMessage(TMessage message) {
    BinarySemaphore semaphore;
    messageAwaiters.emplace(message, &semaphore);
    semaphore.Acquire();
    messageAwaiters.erase(message);
  }

 private:
  std::queue<TMessage> messageQueue;
  std::unordered_map<TMessage, BinarySemaphore*> messageAwaiters;
};

#endif  // VULKAN_SRC_CORE_THREADING_MULTITHREADEDMESSAGEQUEUE_H_
