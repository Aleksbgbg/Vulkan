#ifndef VULKAN_SRC_ENGINE_BEHAVIOUR_BEHAVIOUR_H_
#define VULKAN_SRC_ENGINE_BEHAVIOUR_BEHAVIOUR_H_

#include "Updateable.h"
#include "engine/behaviour/UpdateContext.h"

class Behaviour : public Updateable {
 public:
  virtual ~Behaviour() = default;

  virtual void OnSpawn() {}
  virtual void UpdateModel(const UpdateContext& context) override {}
};

#endif  // VULKAN_SRC_ENGINE_BEHAVIOUR_BEHAVIOUR_H_
