FILE(
    GLOB_RECURSE SRC_LIST 
    ../src/media/*.c
    ../src/media/*.cpp
    ../src/config/*.c
    ../src/config/*.cpp
    ../src/socket/*.c
    ../src/socket/*.cpp
    ../src/utils/*.c
    ../src/utils/*.cpp
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
    # OpenCV
    libopencv_core.so
    libopencv_highgui.so
    libopencv_imgproc.so
    # Json
    libjson-c.so
)