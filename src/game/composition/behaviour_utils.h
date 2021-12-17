#ifndef VULKAN_SRC_GAME_COMPOSITION_BEHAVIOUR_UTILS_H_
#define VULKAN_SRC_GAME_COMPOSITION_BEHAVIOUR_UTILS_H_

#define BEHAVIOUR(type, ...)                           \
  [&](const game::Actor* parent, game::Actor& actor) { \
    return std::make_unique<type>(__VA_ARGS__);        \
  }

#endif  // VULKAN_SRC_GAME_COMPOSITION_BEHAVIOUR_UTILS_H_
