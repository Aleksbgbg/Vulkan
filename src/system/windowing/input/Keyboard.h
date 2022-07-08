#ifndef SRC_SYSTEM_WINDOWING_INPUT_KEYBOARD_H_
#define SRC_SYSTEM_WINDOWING_INPUT_KEYBOARD_H_

#include <unordered_set>

#include "util/include/sdl.h"
// After including main SDL
#include <SDL_keycode.h>

class Keyboard {
 public:
  void KeyDown(const SDL_Keycode keycode);
  void KeyUp(const SDL_Keycode keycode);

  void ClearPressedKeys();

  bool IsKeyDown(const SDL_Keycode keycode) const;
  bool PressedKey(const SDL_Keycode keycode) const;

 private:
  std::unordered_set<SDL_Keycode> keysDown;
  std::unordered_set<SDL_Keycode> pressedKeys;
};

#endif  // SRC_SYSTEM_WINDOWING_INPUT_KEYBOARD_H_
