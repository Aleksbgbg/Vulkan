#ifndef SRC_GAME_BEHAVIOURS_PAUSEMENUTOGGLE_H_
#define SRC_GAME_BEHAVIOURS_PAUSEMENUTOGGLE_H_

#include "engine/behaviour/Behaviour.h"
#include "engine/property/type/GraphicalInterface.h"

class PauseMenuToggle : public Behaviour {
 public:
  PauseMenuToggle(GraphicalInterface& interface);

  void UpdateModel(const UpdateContext& context) override;

 private:
  GraphicalInterface& interface_;
};

#endif  // SRC_GAME_BEHAVIOURS_PAUSEMENUTOGGLE_H_
