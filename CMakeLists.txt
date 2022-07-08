cmake_minimum_required(VERSION 3.20)
project(vulkan)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CORE_LIBRARY_NAME ${CMAKE_PROJECT_NAME}_lib)
set(CORE_BINARY_NAME ${CMAKE_PROJECT_NAME})

set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/lib)
set(GENERATED_FILES_DIR ${CMAKE_SOURCE_DIR}/generated)
set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/resources)

function(add_shader TARGET SHADER_PATH)
  get_filename_component(SHADER ${SHADER_PATH} NAME)

  set(current-shader-path ${CMAKE_SOURCE_DIR}/${SHADER_PATH})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER}.spv)

  # Add a custom command to compile GLSL to SPIR-V.
  find_program(GLSLC glslc PATHS ${DEPENDENCIES_DIR}/vulkan/bin)
  get_filename_component(GLSLC ${GLSLC} ABSOLUTE)

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

function(add_shaders TARGET SHADERS)
  foreach (SHADER IN LISTS SHADERS)
    add_shader(${TARGET} ${SHADER})
  endforeach ()
endfunction()

function(add_files_to_output DIRECTORY SUBPATH)
  file(GLOB FILES LIST_DIRECTORIES false ${DIRECTORY}/*)
  foreach (FILE IN LISTS FILES)
    get_filename_component(NAME ${FILE} NAME)
    configure_file(${FILE} ${SUBPATH}/${NAME} COPYONLY)
  endforeach ()
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

add_subdirectory(${DEPENDENCIES_DIR}/freetype)
add_subdirectory(${DEPENDENCIES_DIR}/googletest)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(profiling)
