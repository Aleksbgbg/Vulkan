#ifndef SRC_ENGINE_BEHAVIOUR_UPDATEABLE_H_
#define SRC_ENGINE_BEHAVIOUR_UPDATEABLE_H_

#include "engine/behaviour/UpdateContext.h"

class Updateable {
 public:
  virtual ~Updateable() = default;

  virtual void UpdateModel(const UpdateContext& context) = 0;
};

#endif  // SRC_ENGINE_BEHAVIOUR_UPDATEABLE_H_
