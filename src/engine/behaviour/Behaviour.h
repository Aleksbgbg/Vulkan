#ifndef SRC_ENGINE_BEHAVIOUR_BEHAVIOUR_H_
#define SRC_ENGINE_BEHAVIOUR_BEHAVIOUR_H_

#include "Updateable.h"
#include "engine/behaviour/UpdateContext.h"

class Behaviour : public Updateable {
 public:
  virtual ~Behaviour() = default;

  virtual void OnSpawn() {}
  virtual void UpdateModel(const UpdateContext& context) override {}
};

#endif  // SRC_ENGINE_BEHAVIOUR_BEHAVIOUR_H_
