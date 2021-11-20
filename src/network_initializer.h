#ifndef VULKAN_SRC_NETWORK_INITIALIZER_H
#define VULKAN_SRC_NETWORK_INITIALIZER_H

#include <memory>

#include "AppNetwork.h"

std::unique_ptr<AppNetwork> CreateNetwork();

#endif  // VULKAN_SRC_NETWORK_INITIALIZER_H
