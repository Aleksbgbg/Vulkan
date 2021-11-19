#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCEBINDER_H
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCEBINDER_H

#include "BufferBinder.h"
#include "TextureBinder.h"

class ResourceBinder : public BufferBinder, public TextureBinder {
 public:
  virtual ~ResourceBinder() = default;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCEBINDER_H
