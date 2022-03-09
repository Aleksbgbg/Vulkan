set(BINARY_NAME profiling)
set(SOURCE_FILES main.cc)
add_executable(${BINARY_NAME} ${SOURCE_FILES})
target_link_libraries(${BINARY_NAME} PRIVATE vulkan_lib)
