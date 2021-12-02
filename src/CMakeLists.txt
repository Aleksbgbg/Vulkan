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

set(SOURCE_FILES App.cc App.h vulkan/VulkanInstance.cc vulkan/VulkanInstance.h vulkan/error.h vulkan/structures/define_structure.h vulkan/structures/ApplicationInfo.h vulkan/structures/InstanceCreateInfo.h util/build_definition.h vulkan/util.h vulkan/DebugUtilsMessenger.cc vulkan/DebugUtilsMessenger.h vulkan/structures/DebugUtilsMessengerCreateInfo.h vulkan/PhysicalDevice.cc vulkan/PhysicalDevice.h vulkan/structures/Win32SurfaceCreateInfo.h vulkan/Surface.cc vulkan/Surface.h vulkan/structures/DeviceQueueCreateInfo.h vulkan/VirtualDevice.cc vulkan/VirtualDevice.h vulkan/structures/DeviceCreateInfo.h vulkan/structures/PhysicalDeviceFeatures.h vulkan/structures/BufferCreateInfo.h vulkan/structures/default.h vulkan/Buffer.cc vulkan/Buffer.h vulkan/DeviceMemory.cc vulkan/DeviceMemory.h vulkan/structures/MemoryAllocateInfo.h vulkan/Queue.cc vulkan/Queue.h vulkan/structures/CommandPoolCreateInfo.h vulkan/CommandPool.cc vulkan/CommandPool.h vulkan/structures/CommandBufferAllocateInfo.h vulkan/structures/CommandBufferBeginInfo.h vulkan/structures/SubmitInfo.h vulkan/CommandBuffer.cc vulkan/CommandBuffer.h vulkan/structures/FenceCreateInfo.h vulkan/Fence.cc vulkan/Fence.h vulkan/structures/DescriptorSetLayoutBinding.h vulkan/structures/DescriptorSetLayoutCreateInfo.h vulkan/structures/PipelineLayoutCreateInfo.h vulkan/structures/SwapchainCreateInfo.h vulkan/structures/ImageViewCreateInfo.h vulkan/structures/ImageCreateInfo.h vulkan/structures/Extent3D.h vulkan/structures/ImageSubresourceRange.h vulkan/structures/ShaderModuleCreateInfo.h vulkan/structures/PipelineShaderStageCreateInfo.h vulkan/structures/VertexInputBindingDescription.h vulkan/structures/VertexInputAttributeDescription.h vulkan/structures/PipelineVertexInputStateCreateInfo.h vulkan/structures/PipelineInputAssemblyStateCreateInfo.h vulkan/structures/Viewport.h vulkan/structures/Extent2D.h vulkan/structures/Offset2D.h vulkan/structures/Rect2D.h vulkan/structures/PipelineViewportStateCreateInfo.h vulkan/structures/PipelineRasterizationStateCreateInfo.h vulkan/structures/PipelineMultisampleStateCreateInfo.h vulkan/structures/PipelineColorBlendAttachmentState.h vulkan/structures/PipelineColorBlendStateCreateInfo.h vulkan/structures/AttachmentDescription.h vulkan/structures/AttachmentReference.h vulkan/structures/SubpassDescription.h vulkan/structures/SubpassDependency.h vulkan/structures/RenderPassCreateInfo.h vulkan/structures/PipelineDepthStencilStateCreateInfo.h vulkan/structures/GraphicsPipelineCreateInfo.h vulkan/structures/FramebufferCreateInfo.h vulkan/structures/DescriptorPoolSize.h vulkan/structures/DescriptorPoolCreateInfo.h vulkan/structures/DescriptorSetAllocateInfo.h vulkan/structures/DescriptorBufferInfo.h vulkan/structures/WriteDescriptorSet.h vulkan/structures/RenderPassBeginInfo.h vulkan/structures/SemaphoreCreateInfo.h vulkan/structures/PresentInfoKHR.h vulkan/Swapchain.cc vulkan/Swapchain.h vulkan/structures/PipelineTessellationStateCreateInfo.h vulkan/Pipeline.cc vulkan/Pipeline.h vulkan/structures/PipelineDynamicStateCreateInfo.h vulkan/ShaderModule.cc vulkan/ShaderModule.h vulkan/PipelineLayout.cc vulkan/PipelineLayout.h vulkan/RenderPass.cc vulkan/RenderPass.h vulkan/DescriptorSetLayout.cc vulkan/DescriptorSetLayout.h vulkan/Image.cc vulkan/Image.h vulkan/Semaphore.cc vulkan/Semaphore.h vulkan/ImageView.cc vulkan/ImageView.h vulkan/Framebuffer.cc vulkan/Framebuffer.h vulkan/structures/ClearValue.h vulkan/structures/ClearDepthStencilValue.h vulkan/structures/ClearColorValue.h vulkan/DescriptorSet.cc vulkan/DescriptorSet.h vulkan/DescriptorPool.cc vulkan/DescriptorPool.h vulkan/SynchronisationPack.cc vulkan/SynchronisationPack.h system/windowing/input/Keyboard.cc system/windowing/input/Keyboard.h general/threading/MultithreadedMessageQueue.h vulkan/structures/BufferImageCopy.h vulkan/structures/ImageSubresourceLayers.h vulkan/structures/ImageMemoryBarrier.h vulkan/structures/SamplerCreateInfo.h vulkan/Sampler.cc vulkan/Sampler.h vulkan/structures/DescriptorImageInfo.h game/rendering/vertices/PositionNormalTextureVertex.h vulkan/structures/PushConstantRange.h util/include/sdl.h util/include/glm.h vulkan/SubpassReference.h memory/DeviceMemoryAllocator.cc memory/DeviceMemoryAllocator.h memory/MemoryAllocation.h memory/DeviceHeap.cc memory/DeviceHeap.h memory/MemoryBlock.h memory/Allocator.h memory/ReservedBlock.cc memory/ReservedBlock.h memory/MemoryObject.h memory/AllocatedBlock.h general/files/images/bmp.h general/files/images/bmp.cc util/include/windows.h vulkan/structures/PipelineCacheCreateInfo.h vulkan/PipelineCache.cc vulkan/PipelineCache.h util/filenames.h general/files/file.h general/files/file.cc general/geometry/Rect.h vulkan/structures/MemoryBarrier.h vulkan/structures/BufferMemoryBarrier.h general/files/obj.cc general/files/obj.h vulkan/structures/MappedMemoryRange.h DynamicUniformBuffer.h memory/Alignment.h memory/Alignment.cc memory/ReservedMemory.cc memory/DeviceMemorySubAllocation.h system/windowing/input/Mouse.cc system/windowing/input/Mouse.h general/files/images/png.h general/files/images/Image.h general/files/images/png.cc general/algorithms/HuffmanTree.h general/algorithms/HuffmanTree.cc general/logging/log.h general/logging/log.cc system/windowing/Window.cc system/windowing/Window.h game/rendering/Renderable.h game/model/Updatable.h game/Actor.h game/model/UpdateContext.h game/rendering/meshes/MeshRenderer.h game/renders/spaceships/SpaceshipModel.cc game/renders/spaceships/SpaceshipModel.h game/rendering/meshes/MeshRenderer.cc BufferWithMemory.h ImageWithMemory.h game/rendering/meshes/Mesh.h game/rendering/resources/ResourceLoader.h game/rendering/resources/ResourceBinder.h game/renders/spaceships/Npc.cc game/renders/spaceships/Npc.h game/renders/spaceships/MainPlayer.cc game/renders/spaceships/MainPlayer.h game/rendering/Texture.h util/include/glm.cc game/RenderPipeline.cc game/RenderPipeline.h game/rendering/vertices/PositionTextureVertex.h game/renders/spaceships/SpaceshipMesh.cc game/renders/spaceships/SpaceshipMesh.h game/renders/sky/Skybox.cc game/renders/sky/Skybox.h game/rendering/meshes/TexturedMesh.cc game/rendering/meshes/TexturedMesh.h game/renders/light/LightBox.cc game/renders/light/LightBox.h game/renders/SceneRenderer.cc game/renders/SceneRenderer.h game/Scene.cc game/Scene.h game/renders/light/LightRender.cc game/renders/light/LightRender.h game/renders/SceneRender.h game/renders/sky/SkyboxRender.cc game/renders/sky/SkyboxRender.h game/renders/DescriptorConfiguration.h game/renders/ActorDescriptorBinder.h game/renders/SceneDescriptorBinder.cc game/renders/SceneDescriptorBinder.h ViewTransform.h game/renders/spaceships/SpaceshipRender.cc game/renders/spaceships/SpaceshipRender.h game/renders/DescriptorConfiguration.cc game/Camera.cc game/Camera.h game/SceneDescriptor.cc game/SceneDescriptor.h general/files/vertex_loader.h game/rendering/vertices/PositionNormalTextureVertex.cc game/rendering/vertices/PositionTextureVertex.cc general/files/DrawVertices.h game/renders/particles/ParticleSpawner.cc game/renders/particles/ParticleSpawner.h game/renders/particles/ParticleInstance.cc game/renders/particles/ParticleInstance.h game/rendering/vertices/PositionVertex.cc game/rendering/vertices/PositionVertex.h game/renders/particles/MultiParticleMesh.cc game/renders/particles/MultiParticleMesh.h general/algorithms/RandomNumberGenerator.cc general/algorithms/RandomNumberGenerator.h game/renders/ParticleStream.h general/geometry/Rect.cc vulkan/SurfaceFactory.h vulkan/structures/XlibSurfaceCreateInfo.h SwapchainWithResources.cc SwapchainWithResources.h Vulkan.cc Vulkan.h game/renders/DescriptorSetLayoutFactory.h general/math/math.h game/renders/spaceships/SpaceshipsActor.cc game/renders/spaceships/SpaceshipsActor.h game/renders/ParticleStream.h game/renders/ParticleController.h game/rendering/resources/BufferBinder.h game/rendering/resources/TextureBinder.h game/renders/ActorSpawnController.h game/renders/ActorDescriptorAllocator.h game/renders/ActorGenerator.h game/renders/spaceships/PlayerController.h game/renders/spaceships/SystemInputPlayerController.cc game/renders/spaceships/SystemInputPlayerController.h game/renders/spaceships/NoMovementPlayerController.cc game/renders/spaceships/NoMovementPlayerController.h game/Controls.h game/Controls.cc game/renders/spaceships/OtherPlayer.cc game/renders/spaceships/OtherPlayer.h system/System.cc system/System.h run.cc run.h VulkanSystem.h)

add_library(${LIBRARY_NAME} ${SOURCE_FILES})
if (WIN32)
  add_executable(${BINARY_NAME} WIN32 windows_main.cc)
elseif (UNIX)
  add_executable(${BINARY_NAME} linux_main.cc)
endif ()
target_link_libraries(${BINARY_NAME} PRIVATE ${LIBRARY_NAME})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/vulkan/include)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/sdl/include)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/glm/include)

target_include_directories(${LIBRARY_NAME} PUBLIC .)
target_include_directories(${BINARY_NAME} PUBLIC .)

if (WIN32)
  link(${BINARY_NAME} ${CMAKE_SOURCE_DIR}/lib/sdl/lib/windows/SDL2.lib)
  link(${BINARY_NAME} ${CMAKE_SOURCE_DIR}/lib/vulkan/lib/windows/vulkan-1.lib)
elseif (UNIX)
  find_package(Threads REQUIRED)
  find_package(SDL2 REQUIRED SDL2)

  target_link_libraries(${LIBRARY_NAME} PRIVATE Threads::Threads)
  target_link_libraries(${BINARY_NAME} PRIVATE SDL2)
  link(${BINARY_NAME} ${CMAKE_SOURCE_DIR}/lib/vulkan/lib/linux/libvulkan.so)

  target_compile_options(${BINARY_NAME} PRIVATE -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lX11)
endif ()

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
