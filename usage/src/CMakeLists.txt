FILE(
    GLOB_RECURSE SRC_LIST 
    ../inc/*.c
    ./*.c
)

# Exe output path
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

ADD_EXECUTABLE(${USAGE_NAME} ${SRC_LIST})

# Link lib and so
TARGET_LINK_LIBRARIES(
    ${USAGE_NAME}
)