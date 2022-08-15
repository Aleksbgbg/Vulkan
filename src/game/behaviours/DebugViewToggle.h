#ifndef SRC_GAME_BEHAVIOURS_DEBUGVIEWTOGGLE_H_
#define SRC_GAME_BEHAVIOURS_DEBUGVIEWTOGGLE_H_

#include "engine/behaviour/Behaviour.h"
#include "engine/property/type/GraphicalInterface.h"

class DebugViewToggle : public Behaviour {
 public:
  DebugViewToggle(GraphicalInterface& interface);

  void UpdateModel(const UpdateContext& context) override;

 private:
  GraphicalInterface& interface_;
};

#endif  // SRC_GAME_BEHAVIOURS_DEBUGVIEWTOGGLE_H_
