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
  # find_program(GLSLC glslc PATHS lib/vulkan/bin)
  set(GLSLC ${CMAKE_SOURCE_DIR}/lib/vulkan/bin/glslc.exe)
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

set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_NAME ${CMAKE_PROJECT_NAME}_lib)
set(BINARY_NAME ${CMAKE_PROJECT_NAME})

set(IMGUI_LIBRARY imgui)
set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_vulkan.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_sdl.cpp)
add_library(${IMGUI_LIBRARY} ${IMGUI_SOURCES})

target_include_directories(${IMGUI_LIBRARY} PUBLIC ${CMAKE_SOURCE_DIR}/lib/imgui)
target_include_directories(${IMGUI_LIBRARY} PUBLIC ${CMAKE_SOURCE_DIR}/lib/vulkan/include)
target_include_directories(${IMGUI_LIBRARY} PUBLIC ${CMAKE_SOURCE_DIR}/lib/sdl/include)

set(SOURCE_FILES App.cpp App.h vulkan/VulkanInstance.cpp vulkan/VulkanInstance.h vulkan/error.h vulkan/structures/define_structure.h vulkan/structures/ApplicationInfo.h vulkan/structures/InstanceCreateInfo.h build_definition.h vulkan/util.h vulkan/DebugUtilsMessenger.cpp vulkan/DebugUtilsMessenger.h vulkan/structures/DebugUtilsMessengerCreateInfo.h vulkan/PhysicalDevice.cpp vulkan/PhysicalDevice.h vulkan/structures/Win32SurfaceCreateInfo.h vulkan/Surface.cpp vulkan/Surface.h vulkan/lifetime_semantics.h vulkan/structures/DeviceQueueCreateInfo.h vulkan/VirtualDevice.cpp vulkan/VirtualDevice.h vulkan/structures/DeviceCreateInfo.h vulkan/structures/PhysicalDeviceFeatures.h vulkan/structures/BufferCreateInfo.h vulkan/structures/default.h vulkan/Buffer.cpp vulkan/Buffer.h vulkan/DeviceMemory.cpp vulkan/DeviceMemory.h vulkan/structures/MemoryAllocateInfo.h vulkan/Queue.cpp vulkan/Queue.h vulkan/structures/CommandPoolCreateInfo.h vulkan/CommandPool.cpp vulkan/CommandPool.h vulkan/structures/CommandBufferAllocateInfo.h vulkan/structures/CommandBufferBeginInfo.h vulkan/structures/SubmitInfo.h vulkan/CommandBuffer.cpp vulkan/CommandBuffer.h vulkan/structures/FenceCreateInfo.h vulkan/Fence.cpp vulkan/Fence.h vulkan/structures/DescriptorSetLayoutBinding.h vulkan/structures/DescriptorSetLayoutCreateInfo.h vulkan/structures/PipelineLayoutCreateInfo.h vulkan/structures/SwapchainCreateInfo.h vulkan/structures/ImageViewCreateInfo.h vulkan/structures/ImageCreateInfo.h vulkan/structures/Extent3D.h vulkan/structures/ImageSubresourceRange.h vulkan/structures/ShaderModuleCreateInfo.h vulkan/structures/PipelineShaderStageCreateInfo.h vulkan/structures/VertexInputBindingDescription.h vulkan/structures/VertexInputAttributeDescription.h vulkan/structures/PipelineVertexInputStateCreateInfo.h vulkan/structures/PipelineInputAssemblyStateCreateInfo.h vulkan/structures/Viewport.h vulkan/structures/Extent2D.h vulkan/structures/Offset2D.h vulkan/structures/Rect2D.h vulkan/structures/PipelineViewportStateCreateInfo.h vulkan/structures/PipelineRasterizationStateCreateInfo.h vulkan/structures/PipelineMultisampleStateCreateInfo.h vulkan/structures/PipelineColorBlendAttachmentState.h vulkan/structures/PipelineColorBlendStateCreateInfo.h vulkan/structures/AttachmentDescription.h vulkan/structures/AttachmentReference.h vulkan/structures/SubpassDescription.h vulkan/structures/SubpassDependency.h vulkan/structures/RenderPassCreateInfo.h vulkan/structures/PipelineDepthStencilStateCreateInfo.h vulkan/structures/GraphicsPipelineCreateInfo.h vulkan/structures/FramebufferCreateInfo.h vulkan/structures/DescriptorPoolSize.h vulkan/structures/DescriptorPoolCreateInfo.h vulkan/structures/DescriptorSetAllocateInfo.h vulkan/structures/DescriptorBufferInfo.h vulkan/structures/WriteDescriptorSet.h vulkan/structures/RenderPassBeginInfo.h vulkan/structures/SemaphoreCreateInfo.h vulkan/structures/PresentInfoKHR.h vulkan/Swapchain.cpp vulkan/Swapchain.h vulkan/structures/PipelineTessellationStateCreateInfo.h vulkan/Pipeline.cpp vulkan/Pipeline.h vulkan/structures/PipelineDynamicStateCreateInfo.h vulkan/ShaderModule.cpp vulkan/ShaderModule.h vulkan/PipelineLayout.cpp vulkan/PipelineLayout.h vulkan/RenderPass.cpp vulkan/RenderPass.h vulkan/DescriptorSetLayout.cpp vulkan/DescriptorSetLayout.h vulkan/Image.cpp vulkan/Image.h vulkan/Semaphore.cpp vulkan/Semaphore.h vulkan/ImageView.cpp vulkan/ImageView.h vulkan/Framebuffer.cpp vulkan/Framebuffer.h vulkan/structures/ClearValue.h vulkan/structures/ClearDepthStencilValue.h vulkan/structures/ClearColorValue.h vulkan/DescriptorSet.cpp vulkan/DescriptorSet.h vulkan/DescriptorPool.cpp vulkan/DescriptorPool.h vulkan/SynchronisationPack.cpp vulkan/SynchronisationPack.h Keyboard.cpp Keyboard.h MultithreadedMessageQueue.h vulkan/structures/BufferImageCopy.h vulkan/structures/ImageSubresourceLayers.h vulkan/structures/ImageMemoryBarrier.h vulkan/structures/SamplerCreateInfo.h vulkan/Sampler.cpp vulkan/Sampler.h vulkan/structures/DescriptorImageInfo.h VertexBuilder.cpp VertexBuilder.h TexturedVertex.h vulkan/structures/PushConstantRange.h UiRenderer.cpp UiRenderer.h vulkan/ImGuiInstance.cpp vulkan/ImGuiInstance.h Iterable.h include_sdl.h include_glm.h GradientVertex.h vulkan/SubpassReference.h GradientCube.cpp GradientCube.h memory/DeviceMemoryAllocator.cpp memory/DeviceMemoryAllocator.h memory/MemoryAllocation.h memory/DeviceHeap.cpp memory/DeviceHeap.h memory/MemoryBlock.h memory/Allocator.h memory/ReservedBlock.cpp memory/ReservedBlock.h memory/MemoryObject.h memory/AllocatedBlock.h read_bitmap.h read_bitmap.cpp include_windows.h vulkan/structures/PipelineCacheCreateInfo.h vulkan/PipelineCache.cpp vulkan/PipelineCache.h filenames.h file.h file.cpp)

add_library(${LIBRARY_NAME} ${SOURCE_FILES})
add_executable(${BINARY_NAME} main.cpp)
target_link_libraries(${LIBRARY_NAME} PRIVATE ${IMGUI_LIBRARY})
target_link_libraries(${BINARY_NAME} PRIVATE ${LIBRARY_NAME})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/vulkan/include)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/sdl/include)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/glm/include)

target_include_directories(${LIBRARY_NAME} PUBLIC .)
target_include_directories(${BINARY_NAME} PUBLIC .)

link(${BINARY_NAME} ${CMAKE_SOURCE_DIR}/lib/sdl/lib/SDL2main.lib)
link(${BINARY_NAME} ${CMAKE_SOURCE_DIR}/lib/sdl/lib/SDL2.lib)
link(${BINARY_NAME} ${CMAKE_SOURCE_DIR}/lib/vulkan/lib/vulkan-1.lib)

copy_files(${CMAKE_SOURCE_DIR}/lib/sdl/bin/* ${CMAKE_CURRENT_BINARY_DIR})
copy_files(${CMAKE_SOURCE_DIR}/resources/* ${CMAKE_CURRENT_BINARY_DIR}/resources)

file(GLOB SHADERS shaders/*)
foreach (shader IN LISTS SHADERS)
  add_shader(${BINARY_NAME} ${shader})
endforeach ()
