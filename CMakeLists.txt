cmake_minimum_required(VERSION 3.20)
project(vulkan)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(add_files_to_output DIRECTORY SUBPATH)
  file(GLOB FILES ${DIRECTORY}/*)
  foreach(FILE IN LISTS FILES)
    get_filename_component(NAME ${FILE} NAME)
    configure_file(${FILE} ${CMAKE_CURRENT_BINARY_DIR}/${SUBPATH}/${NAME} COPYONLY)
  endforeach()
endfunction()

add_subdirectory(lib/googletest)

add_subdirectory(src)
add_subdirectory(test)
