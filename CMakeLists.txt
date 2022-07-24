cmake_minimum_required(VERSION 3.20)
project(yomi)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CORE_LIBRARY_NAME ${CMAKE_PROJECT_NAME}_lib)
set(CORE_BINARY_NAME ${CMAKE_PROJECT_NAME})

set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/dependencies)
set(GENERATED_FILES_DIR ${CMAKE_SOURCE_DIR}/generated)
set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/resources)

set(VULKAN_SDK $ENV{VULKAN_SDK})

if((WIN32 OR APPLE OR UNIX) AND (NOT ANDROID))
  set(DESKTOP TRUE)
elseif(ANDROID)
  set(MOBILE TRUE)
endif()

function(add_shader TARGET SHADER_ABSOLUTE_PATH)
  get_filename_component(SHADER_NAME ${SHADER_ABSOLUTE_PATH} NAME)

  set(SHADER_OUTPUT_PATH ${GENERATED_FILES_DIR}/shaders/${SHADER_NAME}.spv)

  find_program(GLSLC glslc PATHS ${VULKAN_SDK}/Bin)
  get_filename_component(GLSLC ${GLSLC} ABSOLUTE)

  get_filename_component(SHADER_OUTPUT_DIRECTORY ${SHADER_OUTPUT_PATH} DIRECTORY)
  file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIRECTORY})

  # Add a custom command to compile GLSL to SPIR-V.
  add_custom_command(
      OUTPUT ${SHADER_OUTPUT_PATH}
      COMMAND ${GLSLC} -O -o ${SHADER_OUTPUT_PATH} ${SHADER_ABSOLUTE_PATH}
      DEPENDS ${SHADER_ABSOLUTE_PATH}
      IMPLICIT_DEPENDS CXX ${SHADER_ABSOLUTE_PATH}
      VERBATIM)

  set_source_files_properties(${SHADER_OUTPUT_PATH} PROPERTIES GENERATED TRUE)
  target_sources(${TARGET} PRIVATE ${SHADER_OUTPUT_PATH})
endfunction()

function(add_shaders TARGET SHADERS)
  foreach(SHADER IN LISTS SHADERS)
    add_shader(${TARGET} ${SHADER})
  endforeach()
endfunction()

function(add_files_to_output DIRECTORY OUTPUT_PATH)
  copy_directory_files(${DIRECTORY} ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH})
endfunction()

function(copy_directory_files DIRECTORY OUTPUT_PATH)
  file(GLOB FILES LIST_DIRECTORIES false ${DIRECTORY}/*)
  foreach(FILE IN LISTS FILES)
    get_filename_component(NAME ${FILE} NAME)
    configure_file(${FILE} ${OUTPUT_PATH}/${NAME} COPYONLY)
  endforeach()
endfunction()

function(add_ui_page TARGET PAGE_PATH)
  get_filename_component(PAGE_NAME ${PAGE_PATH} NAME)
  set(OUTPUT_PATH ${GENERATED_FILES_DIR}/ui_pages/${PAGE_NAME})
  add_custom_command(
      OUTPUT ${OUTPUT_PATH}
      COMMAND ${CMAKE_COMMAND}
      -D INPUT_PATH=${PAGE_PATH} -D OUTPUT_PATH=${OUTPUT_PATH} -P "${CMAKE_SOURCE_DIR}/file_to_cpp_string.cmake"
      DEPENDS ${PAGE_PATH}
      VERBATIM)
  target_sources(${TARGET} PRIVATE ${OUTPUT_PATH})
endfunction()

set(FT_DISABLE_ZLIB TRUE)
set(FT_DISABLE_BZIP2 TRUE)
set(FT_DISABLE_PNG TRUE)
set(FT_DISABLE_HARFBUZZ TRUE)
set(FT_DISABLE_BROTLI TRUE)

add_subdirectory(${DEPENDENCIES_DIR}/googletest)
add_subdirectory(${DEPENDENCIES_DIR}/sdl)
add_subdirectory(${DEPENDENCIES_DIR}/freetype)

add_subdirectory(src)
add_subdirectory(test)
if(DESKTOP)
  add_subdirectory(profiling)
endif()
