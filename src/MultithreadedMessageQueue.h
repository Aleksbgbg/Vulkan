#ifndef VULKAN_SRC_MULTITHREADEDMESSAGEQUEUE_H
#define VULKAN_SRC_MULTITHREADEDMESSAGEQUEUE_H

#include <queue>
#include <unordered_map>
#include <semaphore>

template<typename TMessage>
class MultithreadedMessageQueue {
public:
  MultithreadedMessageQueue() : messageQueue(), messageAwaiters() {}

  void PostMessage(TMessage message) {
    if (messageAwaiters.contains(message)) {
      std::binary_semaphore* semaphore = messageAwaiters.at(message);
      semaphore->release();
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
    std::binary_semaphore semaphore(0);
    messageAwaiters.emplace(message, &semaphore);
    semaphore.acquire();
    messageAwaiters.erase(message);
  }

private:
  std::queue<TMessage> messageQueue;
  std::unordered_map<TMessage, std::binary_semaphore*> messageAwaiters;
};

#endif // VULKAN_SRC_MULTITHREADEDMESSAGEQUEUE_H
