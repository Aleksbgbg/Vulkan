set(TEST_BINARY_NAME ${CMAKE_PROJECT_NAME}_test)

set(TEST_SOURCES main.cc memory/DeviceHeapTest.cc general/files/images/ReadPngTest.cc general/algorithms/HuffmanTreeTest.cc general/files/images/ReadBmpTest.cc general/files/ReadObjTest.cc)

add_executable(${TEST_BINARY_NAME} ${TEST_SOURCES})
add_test(NAME ${TEST_BINARY_NAME} COMMAND ${TEST_BINARY_NAME})

target_link_libraries(${TEST_BINARY_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_lib gtest gmock)

add_files_to_output(data/images ${CMAKE_CURRENT_BINARY_DIR}/data/images)
