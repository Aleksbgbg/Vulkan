cmake_minimum_required(VERSION 3.17)
project(vulkan)

function(link TARGET LIB)
  get_filename_component(lib-full-path ${LIB} ABSOLUTE)
  target_link_libraries(${TARGET} PRIVATE ${lib-full-path})
endfunction()

function(copy_files GLOB_PATTERN DESTINATION)
  file(GLOB FILES ${GLOB_PATTERN})
  file(COPY ${FILES} DESTINATION ${DESTINATION})
endfunction()

function(add_shader TARGET SHADER_FULL_PATH)
  find_program(GLSLC glslc PATHS ${CMAKE_SOURCE_DIR}/lib/vulkan/bin)
  get_filename_component(GLSLC ${GLSLC} ABSOLUTE)

  get_filename_component(SHADER ${SHADER_FULL_PATH} NAME)

  set(current-shader-path ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER}.spv)

  # Add a custom command to compile GLSL to SPIR-V.
  get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
  file(MAKE_DIRECTORY ${current-output-dir})

  add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${GLSLC} -O -o ${current-output-path} ${current-shader-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)

  # Make sure our build depends on this output.
  set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
  target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction()

set(LIBRARY_NAME ${CMAKE_PROJECT_NAME}_lib)
set(BINARY_NAME ${CMAKE_PROJECT_NAME})

set(SOURCE_FILES App.cpp App.h vulkan/VulkanInstance.cpp vulkan/VulkanInstance.h vulkan/error.h vulkan/structures/define_structure.h vulkan/structures/ApplicationInfo.h vulkan/structures/InstanceCreateInfo.h util/build_definition.h vulkan/util.h vulkan/DebugUtilsMessenger.cpp vulkan/DebugUtilsMessenger.h vulkan/structures/DebugUtilsMessengerCreateInfo.h vulkan/PhysicalDevice.cpp vulkan/PhysicalDevice.h vulkan/structures/Win32SurfaceCreateInfo.h vulkan/Surface.cpp vulkan/Surface.h vulkan/structures/DeviceQueueCreateInfo.h vulkan/VirtualDevice.cpp vulkan/VirtualDevice.h vulkan/structures/DeviceCreateInfo.h vulkan/structures/PhysicalDeviceFeatures.h vulkan/structures/BufferCreateInfo.h vulkan/structures/default.h vulkan/Buffer.cpp vulkan/Buffer.h vulkan/DeviceMemory.cpp vulkan/DeviceMemory.h vulkan/structures/MemoryAllocateInfo.h vulkan/Queue.cpp vulkan/Queue.h vulkan/structures/CommandPoolCreateInfo.h vulkan/CommandPool.cpp vulkan/CommandPool.h vulkan/structures/CommandBufferAllocateInfo.h vulkan/structures/CommandBufferBeginInfo.h vulkan/structures/SubmitInfo.h vulkan/CommandBuffer.cpp vulkan/CommandBuffer.h vulkan/structures/FenceCreateInfo.h vulkan/Fence.cpp vulkan/Fence.h vulkan/structures/DescriptorSetLayoutBinding.h vulkan/structures/DescriptorSetLayoutCreateInfo.h vulkan/structures/PipelineLayoutCreateInfo.h vulkan/structures/SwapchainCreateInfo.h vulkan/structures/ImageViewCreateInfo.h vulkan/structures/ImageCreateInfo.h vulkan/structures/Extent3D.h vulkan/structures/ImageSubresourceRange.h vulkan/structures/ShaderModuleCreateInfo.h vulkan/structures/PipelineShaderStageCreateInfo.h vulkan/structures/VertexInputBindingDescription.h vulkan/structures/VertexInputAttributeDescription.h vulkan/structures/PipelineVertexInputStateCreateInfo.h vulkan/structures/PipelineInputAssemblyStateCreateInfo.h vulkan/structures/Viewport.h vulkan/structures/Extent2D.h vulkan/structures/Offset2D.h vulkan/structures/Rect2D.h vulkan/structures/PipelineViewportStateCreateInfo.h vulkan/structures/PipelineRasterizationStateCreateInfo.h vulkan/structures/PipelineMultisampleStateCreateInfo.h vulkan/structures/PipelineColorBlendAttachmentState.h vulkan/structures/PipelineColorBlendStateCreateInfo.h vulkan/structures/AttachmentDescription.h vulkan/structures/AttachmentReference.h vulkan/structures/SubpassDescription.h vulkan/structures/SubpassDependency.h vulkan/structures/RenderPassCreateInfo.h vulkan/structures/PipelineDepthStencilStateCreateInfo.h vulkan/structures/GraphicsPipelineCreateInfo.h vulkan/structures/FramebufferCreateInfo.h vulkan/structures/DescriptorPoolSize.h vulkan/structures/DescriptorPoolCreateInfo.h vulkan/structures/DescriptorSetAllocateInfo.h vulkan/structures/DescriptorBufferInfo.h vulkan/structures/WriteDescriptorSet.h vulkan/structures/RenderPassBeginInfo.h vulkan/structures/SemaphoreCreateInfo.h vulkan/structures/PresentInfoKHR.h vulkan/Swapchain.cpp vulkan/Swapchain.h vulkan/structures/PipelineTessellationStateCreateInfo.h vulkan/Pipeline.cpp vulkan/Pipeline.h vulkan/structures/PipelineDynamicStateCreateInfo.h vulkan/ShaderModule.cpp vulkan/ShaderModule.h vulkan/PipelineLayout.cpp vulkan/PipelineLayout.h vulkan/RenderPass.cpp vulkan/RenderPass.h vulkan/DescriptorSetLayout.cpp vulkan/DescriptorSetLayout.h vulkan/Image.cpp vulkan/Image.h vulkan/Semaphore.cpp vulkan/Semaphore.h vulkan/ImageView.cpp vulkan/ImageView.h vulkan/Framebuffer.cpp vulkan/Framebuffer.h vulkan/structures/ClearValue.h vulkan/structures/ClearDepthStencilValue.h vulkan/structures/ClearColorValue.h vulkan/DescriptorSet.cpp vulkan/DescriptorSet.h vulkan/DescriptorPool.cpp vulkan/DescriptorPool.h vulkan/SynchronisationPack.cpp vulkan/SynchronisationPack.h general/windowing/input/Keyboard.cpp general/windowing/input/Keyboard.h general/threading/MultithreadedMessageQueue.h vulkan/structures/BufferImageCopy.h vulkan/structures/ImageSubresourceLayers.h vulkan/structures/ImageMemoryBarrier.h vulkan/structures/SamplerCreateInfo.h vulkan/Sampler.cpp vulkan/Sampler.h vulkan/structures/DescriptorImageInfo.h game/rendering/vertices/PositionNormalTextureVertex.h vulkan/structures/PushConstantRange.h util/include/sdl.h util/include/glm.h vulkan/SubpassReference.h memory/DeviceMemoryAllocator.cpp memory/DeviceMemoryAllocator.h memory/MemoryAllocation.h memory/DeviceHeap.cpp memory/DeviceHeap.h memory/MemoryBlock.h memory/Allocator.h memory/ReservedBlock.cpp memory/ReservedBlock.h memory/MemoryObject.h memory/AllocatedBlock.h general/files/images/bmp.h general/files/images/bmp.cpp util/include/windows.h vulkan/structures/PipelineCacheCreateInfo.h vulkan/PipelineCache.cpp vulkan/PipelineCache.h util/filenames.h general/files/file.h general/files/file.cpp general/geometry/Rect.h vulkan/structures/MemoryBarrier.h vulkan/structures/BufferMemoryBarrier.h general/animations/UpAndDownGlidingAnimation.h general/animations/UpAndDownGlidingAnimation.cpp general/animations/NormalizedOneTimeFunctionAnimation.cpp general/animations/NormalizedOneTimeFunctionAnimation.h general/files/obj.cpp general/files/obj.h vulkan/structures/MappedMemoryRange.h DynamicUniformBuffer.h memory/Alignment.h memory/Alignment.cpp memory/ReservedMemory.cpp memory/DeviceMemorySubAllocation.h general/windowing/input/Mouse.cpp general/windowing/input/Mouse.h general/files/images/png.h general/files/images/Image.h general/files/images/png.cpp general/algorithms/HuffmanTree.h general/algorithms/HuffmanTree.cpp general/logging/log.h general/logging/log.cpp general/windowing/Window.cpp general/windowing/Window.h game/rendering/Renderable.h game/model/Updatable.h game/Actor.h game/model/UpdateContext.h game/rendering/meshes/MeshRenderer.h game/renders/spaceships/SpaceshipModel.cpp game/renders/spaceships/SpaceshipModel.h game/rendering/meshes/MeshRenderer.cpp BufferWithMemory.h ImageWithMemory.h game/rendering/meshes/Mesh.h game/rendering/resources/ResourceLoader.h game/rendering/resources/ResourceBinder.cpp game/rendering/resources/ResourceBinder.h game/renders/spaceships/Npc.cpp game/renders/spaceships/Npc.h game/renders/spaceships/Player.cpp game/renders/spaceships/Player.h game/rendering/Texture.h util/include/glm.cpp game/RenderPipeline.cpp game/RenderPipeline.h game/rendering/vertices/PositionTextureVertex.h game/renders/spaceships/SpaceshipMesh.cpp game/renders/spaceships/SpaceshipMesh.h game/renders/sky/Skybox.cpp game/renders/sky/Skybox.h game/rendering/meshes/TexturedMesh.cpp game/rendering/meshes/TexturedMesh.h game/renders/light/LightBox.cpp game/renders/light/LightBox.h game/renders/SceneRenderer.cpp game/renders/SceneRenderer.h game/Scene.cpp game/Scene.h game/renders/light/LightRender.cpp game/renders/light/LightRender.h game/renders/SceneRender.h game/renders/sky/SkyboxRender.cpp game/renders/sky/SkyboxRender.h game/renders/DescriptorConfiguration.h game/renders/ActorDescriptorBinder.h game/renders/SceneDescriptorBinder.cpp game/renders/SceneDescriptorBinder.h ViewTransform.h game/renders/spaceships/SpaceshipRender.cpp game/renders/spaceships/SpaceshipRender.h game/renders/DescriptorConfiguration.cpp game/Camera.cpp game/Camera.h game/SceneDescriptor.cpp game/SceneDescriptor.h general/files/vertex_loader.h game/rendering/vertices/PositionNormalTextureVertex.cpp game/rendering/vertices/PositionTextureVertex.cpp general/files/DrawVertices.h game/renders/particles/ParticleRender.cpp game/renders/particles/ParticleRender.h game/renders/particles/ParticleGenerator.cpp game/renders/particles/ParticleGenerator.h game/rendering/vertices/PositionVertex.cpp game/rendering/vertices/PositionVertex.h game/renders/particles/MultiParticleMesh.cpp game/renders/particles/MultiParticleMesh.h general/algorithms/RandomNumberGenerator.cpp general/algorithms/RandomNumberGenerator.h game/renders/ParticleController.h general/geometry/Rect.cpp general/windowing/system_window.cpp general/windowing/system_window.h general/windowing/windows/WindowsWindow.cpp general/windowing/windows/WindowsWindow.h general/windowing/linux/LinuxWindow.cpp general/windowing/linux/LinuxWindow.h general/windowing/VulkanWindow.h vulkan/structures/XlibSurfaceCreateInfo.h SwapchainWithResources.cpp SwapchainWithResources.h Vulkan.cpp Vulkan.h game/renders/DescriptorSetLayoutFactory.h general/math/math.h)

add_library(${LIBRARY_NAME} ${SOURCE_FILES})
add_executable(${BINARY_NAME} main.cpp)
target_link_libraries(${LIBRARY_NAME} PRIVATE ${IMGUI_LIBRARY})
target_link_libraries(${BINARY_NAME} PRIVATE ${LIBRARY_NAME})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/vulkan/include)
if(NOT UNIX)
  target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/sdl/include)
endif()
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/glm/include)

target_include_directories(${LIBRARY_NAME} PUBLIC .)
target_include_directories(${BINARY_NAME} PUBLIC .)

if (WIN32)
  link(${BINARY_NAME} ${CMAKE_SOURCE_DIR}/lib/sdl/lib/windows/SDL2main.lib)
  link(${BINARY_NAME} ${CMAKE_SOURCE_DIR}/lib/sdl/lib/windows/SDL2.lib)
  link(${BINARY_NAME} ${CMAKE_SOURCE_DIR}/lib/vulkan/lib/windows/vulkan-1.lib)
elseif(UNIX)
  find_package(Threads REQUIRED)
  find_package(SDL2 REQUIRED SDL2)

  target_link_libraries(${LIBRARY_NAME} PRIVATE Threads::Threads)
  target_link_libraries(${BINARY_NAME} PRIVATE SDL2)
  link(${BINARY_NAME} ${CMAKE_SOURCE_DIR}/lib/vulkan/lib/linux/libvulkan.so)

  target_compile_options(${BINARY_NAME} PRIVATE -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lX11)
endif()

add_files_to_output(${CMAKE_SOURCE_DIR}/lib/sdl/bin .)
add_files_to_output(${CMAKE_SOURCE_DIR}/resources resources)

add_shader(${BINARY_NAME} shaders/light.frag)
add_shader(${BINARY_NAME} shaders/light.vert)
add_shader(${BINARY_NAME} shaders/particles.frag)
add_shader(${BINARY_NAME} shaders/particles.vert)
add_shader(${BINARY_NAME} shaders/sky.frag)
add_shader(${BINARY_NAME} shaders/sky.vert)
add_shader(${BINARY_NAME} shaders/spaceship.frag)
add_shader(${BINARY_NAME} shaders/spaceship.vert)
