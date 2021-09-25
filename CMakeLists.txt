cmake_minimum_required(VERSION 3.17)
project(vulkan)

set(CMAKE_CXX_STANDARD 20)

function(link TARGET LIB)
    get_filename_component(lib-full-path ${LIB} ABSOLUTE)
    target_link_libraries(${TARGET} ${lib-full-path})
endfunction()

function(add_binary_output GLOB_PATTERN)
    file(GLOB FILES ${GLOB_PATTERN})
    file(COPY ${FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

function(add_shader TARGET SHADER)
    # find_program(GLSLC glslc PATHS lib/vulkan/bin)
    set(GLSLC lib/vulkan/bin/glslc.exe)
    get_filename_component(GLSLC ${GLSLC} ABSOLUTE)

    set(current-shader-path ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/${SHADER})
    set(current-output-path ${CMAKE_BINARY_DIR}/shaders/${SHADER}.spv)

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

add_executable(vulkan src/main.cpp src/App.cpp src/App.h src/vulkan/VulkanInstance.cpp src/vulkan/VulkanInstance.h src/vulkan/error.h src/vulkan/structures/define_structure.h src/vulkan/structures/ApplicationInfo.h src/vulkan/structures/InstanceCreateInfo.h src/build_definition.h src/vulkan/util.h src/vulkan/DebugUtilsMessenger.cpp src/vulkan/DebugUtilsMessenger.h src/vulkan/structures/DebugUtilsMessengerCreateInfo.h src/vulkan/PhysicalDevice.cpp src/vulkan/PhysicalDevice.h src/vulkan/structures/Win32SurfaceCreateInfo.h src/vulkan/Surface.cpp src/vulkan/Surface.h src/vulkan/lifetime_semantics.h src/vulkan/structures/DeviceQueueCreateInfo.h src/vulkan/VirtualDevice.cpp src/vulkan/VirtualDevice.h src/vulkan/structures/DeviceCreateInfo.h src/vulkan/structures/PhysicalDeviceFeatures.h src/vulkan/structures/BufferCreateInfo.h src/vulkan/structures/default.h src/vulkan/Buffer.cpp src/vulkan/Buffer.h src/vulkan/DeviceMemory.cpp src/vulkan/DeviceMemory.h src/vulkan/structures/MemoryAllocateInfo.h src/vulkan/Queue.cpp src/vulkan/Queue.h src/vulkan/structures/CommandPoolCreateInfo.h src/vulkan/CommandPool.cpp src/vulkan/CommandPool.h src/vulkan/structures/CommandBufferAllocateInfo.h src/vulkan/structures/CommandBufferBeginInfo.h src/vulkan/structures/SubmitInfo.h src/vulkan/CommandBuffer.cpp src/vulkan/CommandBuffer.h src/vulkan/structures/FenceCreateInfo.h src/vulkan/Fence.cpp src/vulkan/Fence.h src/vulkan/structures/DescriptorSetLayoutBinding.h src/vulkan/structures/DescriptorSetLayoutCreateInfo.h src/vulkan/structures/PipelineLayoutCreateInfo.h src/vulkan/structures/SwapchainCreateInfo.h src/vulkan/structures/ImageViewCreateInfo.h src/vulkan/structures/ImageCreateInfo.h src/vulkan/structures/Extent3D.h src/vulkan/structures/ImageSubresourceRange.h src/vulkan/structures/ShaderModuleCreateInfo.h src/vulkan/structures/PipelineShaderStageCreateInfo.h src/vulkan/structures/VertexInputBindingDescription.h src/vulkan/structures/VertexInputAttributeDescription.h src/vulkan/structures/PipelineVertexInputStateCreateInfo.h src/vulkan/structures/PipelineInputAssemblyStateCreateInfo.h src/vulkan/structures/Viewport.h src/vulkan/structures/Extent2D.h src/vulkan/structures/Offset2D.h src/vulkan/structures/Rect2D.h src/vulkan/structures/PipelineViewportStateCreateInfo.h src/vulkan/structures/PipelineRasterizationStateCreateInfo.h src/vulkan/structures/PipelineMultisampleStateCreateInfo.h src/vulkan/structures/PipelineColorBlendAttachmentState.h src/vulkan/structures/PipelineColorBlendStateCreateInfo.h src/vulkan/structures/AttachmentDescription.h src/vulkan/structures/AttachmentReference.h src/vulkan/structures/SubpassDescription.h src/vulkan/structures/SubpassDependency.h src/vulkan/structures/RenderPassCreateInfo.h src/vulkan/structures/PipelineDepthStencilStateCreateInfo.h src/vulkan/structures/GraphicsPipelineCreateInfo.h src/vulkan/structures/FramebufferCreateInfo.h src/vulkan/structures/DescriptorPoolSize.h src/vulkan/structures/DescriptorPoolCreateInfo.h src/vulkan/structures/DescriptorSetAllocateInfo.h src/vulkan/structures/DescriptorBufferInfo.h src/vulkan/structures/WriteDescriptorSet.h src/vulkan/structures/RenderPassBeginInfo.h src/vulkan/structures/SemaphoreCreateInfo.h src/vulkan/structures/PresentInfoKHR.h src/vulkan/Swapchain.cpp src/vulkan/Swapchain.h src/vulkan/structures/PipelineTessellationStateCreateInfo.h src/vulkan/Pipeline.cpp src/vulkan/Pipeline.h src/vulkan/structures/PipelineDynamicStateCreateInfo.h src/vulkan/ShaderModule.cpp src/vulkan/ShaderModule.h src/vulkan/PipelineLayout.cpp src/vulkan/PipelineLayout.h src/vulkan/RenderPass.cpp src/vulkan/RenderPass.h src/vulkan/DescriptorSetLayout.cpp src/vulkan/DescriptorSetLayout.h src/vulkan/Image.cpp src/vulkan/Image.h src/vulkan/Semaphore.cpp src/vulkan/Semaphore.h src/vulkan/ImageView.cpp src/vulkan/ImageView.h src/vulkan/Framebuffer.cpp src/vulkan/Framebuffer.h src/vulkan/structures/ClearValue.h src/vulkan/structures/ClearDepthStencilValue.h src/vulkan/structures/ClearColorValue.h src/vulkan/DescriptorSet.cpp src/vulkan/DescriptorSet.h src/vulkan/DescriptorPool.cpp src/vulkan/DescriptorPool.h src/vulkan/SynchronisationPack.cpp src/vulkan/SynchronisationPack.h src/Keyboard.cpp src/Keyboard.h src/MultithreadedMessageQueue.h src/vulkan/structures/BufferImageCopy.h src/vulkan/structures/ImageSubresourceLayers.h src/vulkan/structures/ImageMemoryBarrier.h src/vulkan/util.cpp src/vulkan/structures/SamplerCreateInfo.h src/vulkan/Sampler.cpp src/vulkan/Sampler.h src/vulkan/structures/DescriptorImageInfo.h src/VertexBuilder.cpp src/VertexBuilder.h src/Vertex.h)

include_directories(lib/glm/include)
include_directories(lib/sdl/include)
include_directories(lib/vulkan/include)
include_directories(src)

link(vulkan lib/sdl/lib/SDL2.lib)
link(vulkan lib/vulkan/lib/vulkan-1.lib)

add_binary_output("lib/sdl/bin/*")
add_binary_output(resources/*)

add_shader(vulkan shader.vert)
add_shader(vulkan shader.frag)

# file(GLOB VERTEX_SHADERS "src/*.vert")
# file(GLOB FRAGMENT_SHADERS "src/*.frag")