#include "CompositeResource.h"

class CompositeResourceInstance : public Resource {
 public:
  CompositeResourceInstance(std::list<std::unique_ptr<Resource>> resources)
      : resources_(std::move(resources)) {}

 private:
  std::list<std::unique_ptr<Resource>> resources_;
};

std::unique_ptr<Resource> CompositeResource(
    std::list<std::unique_ptr<Resource>> resources) {
  return std::make_unique<CompositeResourceInstance>(std::move(resources));
}
