#ifndef VULKAN_SRC_KEYBOARD_H
#define VULKAN_SRC_KEYBOARD_H

#include <SDL.h>
#include <SDL_keycode.h>

#include <unordered_set>

class Keyboard {
 public:
  void Keydown(const SDL_Keycode keycode);
  void Keyup(const SDL_Keycode keycode);

  void ClearPressedKeys();

  bool IsKeyDown(const SDL_Keycode keycode) const;
  bool PressedKey(const SDL_Keycode keycode) const;

 private:
  std::unordered_set<SDL_Keycode> keysDown;
  std::unordered_set<SDL_Keycode> pressedKeys;
};

#endif  // VULKAN_SRC_KEYBOARD_H
