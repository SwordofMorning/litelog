FILE(
    GLOB_RECURSE SRC_LIST 
    ../src/config/*.c
    ../src/config/*.cpp
    ../src/socket/*.c
    ../src/socket/*.cpp
    ../src/buffer/*.c
    ../src/buffer/*.cpp
    ./*.c
    ./*.hpp
    ./*.cpp
)

LINK_DIRECTORIES(../src)

INCLUDE_DIRECTORIES(../src)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR/bin})

ADD_EXECUTABLE(${TEST_NAME} ${SRC_LIST})

TARGET_LINK_LIBRARIES(
    ${TEST_NAME}
)