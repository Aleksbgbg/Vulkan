#include "Keyboard.h"

void Keyboard::Keydown(const SDL_Keycode keycode) {
  pressedKeys.insert(keycode);
}

void Keyboard::Keyup(const SDL_Keycode keycode) {
  pressedKeys.erase(keycode);
}

bool Keyboard::IsKeyDown(const SDL_Keycode keycode) const {
  return pressedKeys.contains(keycode);
}
