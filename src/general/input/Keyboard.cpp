#include "Keyboard.h"

void Keyboard::Keydown(const SDL_Keycode keycode) {
  keysDown.insert(keycode);
  pressedKeys.insert(keycode);
}

void Keyboard::Keyup(const SDL_Keycode keycode) {
  keysDown.erase(keycode);
}

void Keyboard::ClearPressedKeys() {
  pressedKeys.clear();
}

bool Keyboard::IsKeyDown(const SDL_Keycode keycode) const {
  return keysDown.contains(keycode);
}

bool Keyboard::PressedKey(const SDL_Keycode keycode) const {
  return pressedKeys.contains(keycode);
}
