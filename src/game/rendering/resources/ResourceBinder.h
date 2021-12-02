#ifndef VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCEBINDER_H_
#define VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCEBINDER_H_

#include "BufferBinder.h"
#include "TextureBinder.h"

class ResourceBinder : public BufferBinder, public TextureBinder {
 public:
  virtual ~ResourceBinder() = default;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RESOURCES_RESOURCEBINDER_H_
