set(TEST_BINARY_NAME ${CMAKE_PROJECT_NAME}_test)

set(TEST_SOURCES main.cc memory/DeviceHeapTest.cc core/files/images/ReadPngTest.cc core/algorithms/HuffmanTreeTest.cc core/files/images/ReadBmpTest.cc core/files/ReadObjTest.cc game/ui/ParseXmlTreeTest.cc matchers.h)

add_executable(${TEST_BINARY_NAME} ${TEST_SOURCES})
add_test(NAME ${TEST_BINARY_NAME} COMMAND ${TEST_BINARY_NAME})

target_link_libraries(${TEST_BINARY_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_lib gtest gmock)
target_include_directories(${TEST_BINARY_NAME} PRIVATE .)

add_files_to_output(data/images ${CMAKE_CURRENT_BINARY_DIR}/data/images)
