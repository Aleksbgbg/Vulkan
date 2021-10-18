set(TEST_BINARY_NAME ${CMAKE_PROJECT_NAME}_test)

set(TEST_SOURCES main.cpp memory/DeviceHeapTest.cpp general/files/images/ReadPngTest.cpp general/algorithms/HuffmanTreeTest.cpp general/files/images/ReadBmpTest.cpp)

add_executable(${TEST_BINARY_NAME} ${TEST_SOURCES})
add_test(NAME ${TEST_BINARY_NAME} COMMAND ${TEST_BINARY_NAME})

target_link_libraries(${TEST_BINARY_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_lib gtest gmock)

function(copy_test_image NAME)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/images/${NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/images)
endfunction()

copy_test_image(BasicBitmap.bmp)
copy_test_image(SpaceshipTexture.bmp)
copy_test_image(SpaceshipTexture.png)
copy_test_image(Alice.bmp)
copy_test_image(Alice.png)
