FILE(
    GLOB_RECURSE SRC_LIST 
    ../src/*/*.c
    ../src/*/*.cpp
    ./*.c
    ./*.hpp
    ./*.cpp
)

LIST(REMOVE_ITEM SRC_LIST "../src/main.cpp") 

LINK_DIRECTORIES(../src)

INCLUDE_DIRECTORIES(../src)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR/bin})

ADD_EXECUTABLE(${TEST_NAME} ${SRC_LIST})

TARGET_LINK_LIBRARIES(
    ${TEST_NAME}
)