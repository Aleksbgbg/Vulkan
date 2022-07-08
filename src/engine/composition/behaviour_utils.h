#ifndef SRC_ENGINE_COMPOSITION_BEHAVIOUR_UTILS_H_
#define SRC_ENGINE_COMPOSITION_BEHAVIOUR_UTILS_H_

#define BEHAVIOUR(TYPE, ...)                           \
  [&](game::Actor* const parent, game::Actor& actor) { \
    return std::make_unique<TYPE>(__VA_ARGS__);        \
  }

#endif  // SRC_ENGINE_COMPOSITION_BEHAVIOUR_UTILS_H_
