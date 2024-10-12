#!/bin/bash

#################### Section 1 : Echo ####################

RED='\033[1;31m'
CYAN='\033[1;36m'
GREEN='\033[1;32m'
NC='\033[0m'

function echo_error()
{
    local string="$1"
    echo -e "${CYAN}${ScriptEchoLabel}${RED}${1}${NC}"
}

function echo_info()
{
    local string="$1"
    echo -e "${CYAN}${ScriptEchoLabel}${GREEN}${1}${NC}"
}

#################### Section 2 : Project Setting ####################

# Project Default Parameters
BuildPath="./build"

ProjectPath=$(cd $(dirname $0);pwd)
ScriptEchoLabel="Build.sh: "

#################### Section 3 : Build Project ####################

# if no BuildPath, then Create
if [ ! -d "$BuildPath" ]; then
    mkdir -p "$BuildPath"
    echo_info "Create Build Path: $BuildPath"
fi

# camke ..
function run_cmake()
{
    echo_info "cmake .."
    if cmake ..; then
        echo_info "CMake Success."
    else
        echo_error "CMake Fail."
        exit 1
    fi
}

# make
function run_make() 
{
    echo_info "make -j12"
    if make -j12; then
        echo_info "Make Success."
    else 
        echo_error "Make Fail."
        exit 1
    fi
}

#################### Section 4 : Main ####################

function main() 
{
    echo_info "PWD is: ${ProjectPath}"
    cd "$BuildPath"
    run_cmake
    run_make
}

# run main
main