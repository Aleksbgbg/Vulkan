#ifndef VULKAN_SRC_GAME_ACTOR_COMPOSITERESOURCE_H_
#define VULKAN_SRC_GAME_ACTOR_COMPOSITERESOURCE_H_

#include <array>
#include <list>
#include <memory>

#include "game/actor/resource/Resource.h"

std::unique_ptr<Resource> CompositeResource(
    std::list<std::unique_ptr<Resource>> resources);

template <std::size_t TCount>
std::unique_ptr<Resource> CompositeResource(
    std::array<std::unique_ptr<Resource>, TCount> resources) {
  std::list<std::unique_ptr<Resource>> resourceList;
  for (std::unique_ptr<Resource>& value : resources) {
    resourceList.push_back(std::move(value));
  }
  return CompositeResource(std::move(resourceList));
}

#endif  // VULKAN_SRC_GAME_ACTOR_COMPOSITERESOURCE_H_
