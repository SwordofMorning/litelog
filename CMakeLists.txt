PROJECT(Litelog)

CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# ===== Setp 1 : Set Cross Compiler Path =====

SET(CMAKE_C_COMPILER aarch64-none-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER aarch64-none-linux-gnu-c++)

# ===== Setp 2 : Set Flags =====

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -O3 -lrt -pthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O3 -lrt -pthread -std=c++17")

# ===== Setp 3 : Set Application Name =====

SET(APP_NAME "litelog")
SET(TEST_NAME "litelog_test")
SET(USAGE_NAME "litelog_usage")

# ===== Setp 4 : Add Subdirectory =====

ADD_SUBDIRECTORY(src bin)
# ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(usage/src)

# ===== Step 5: Generate Version Info =====

# Branch
EXECUTE_PROCESS(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Last commit ID
EXECUTE_PROCESS(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_ID
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# User
EXECUTE_PROCESS(
    COMMAND git config user.name
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_USER
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/others/version/version.h.in
    ${CMAKE_SOURCE_DIR}/others/version/version.h
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/others/)

# ===== Step 6: Copy Conf to Output =====

FILE(COPY ${CMAKE_SOURCE_DIR}/others/ini/litelog.ini
    DESTINATION ${CMAKE_BINARY_DIR}/bin)