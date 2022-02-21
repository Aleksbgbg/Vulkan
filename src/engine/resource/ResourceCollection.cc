#include "ResourceCollection.h"

namespace game {

void ResourceCollection::push_back(std::unique_ptr<Resource> resource) {
  resources_.push_back(std::move(resource));
}

}  // namespace game
