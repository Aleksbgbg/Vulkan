cmake_minimum_required(VERSION 3.17)
project(vulkan)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/googletest)

add_subdirectory(src)
add_subdirectory(test)
