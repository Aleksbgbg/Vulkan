#ifndef VULKAN_SRC_SERVER_INPUTS_H
#define VULKAN_SRC_SERVER_INPUTS_H

enum class Type {
  Start = 1,
  Stop,
};

enum class Input {
  Movement = 1,
  Accelerate,
  Decelerate,
};

#endif  // VULKAN_SRC_SERVER_INPUTS_H
