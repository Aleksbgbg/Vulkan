#ifndef SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_GLOBALCOMPUTEUNIFORM_H_
#define SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_GLOBALCOMPUTEUNIFORM_H_

struct GlobalComputeUniform {
  alignas(4) float deltaTime;
};

#endif  // SRC_RENDERER_VULKAN_BUFFER_STRUCTURES_GLOBALCOMPUTEUNIFORM_H_
