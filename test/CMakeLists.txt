set(TEST_BINARY_NAME ${CMAKE_PROJECT_NAME}_test)

set(TEST_SOURCES main.cpp memory/DeviceHeapTest.cpp)

add_executable(${TEST_BINARY_NAME} ${TEST_SOURCES})
add_test(NAME ${TEST_BINARY_NAME} COMMAND ${TEST_BINARY_NAME})

target_link_libraries(${TEST_BINARY_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_lib gtest gmock)
