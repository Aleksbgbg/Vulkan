set(TEST_BINARY_NAME ${CMAKE_PROJECT_NAME}_test)

set(TEST_SOURCES main.cc memory/DeviceHeapTest.cc core/files/png/png_test.cc core/files/png/HuffmanCode_test.cc core/files/ReadBmpTest.cc core/files/ReadObjTest.cc engine/ui/parse_xml_tree_test.cc matchers.h core/files/png/decompress_deflate_test.cc)

add_executable(${TEST_BINARY_NAME} ${TEST_SOURCES})
add_test(NAME ${TEST_BINARY_NAME} COMMAND ${TEST_BINARY_NAME})

target_link_libraries(${TEST_BINARY_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_lib gtest gmock)
target_include_directories(${TEST_BINARY_NAME} PRIVATE .)

add_files_to_output(data/images ${CMAKE_CURRENT_BINARY_DIR}/data/images)
