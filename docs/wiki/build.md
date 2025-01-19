# Build

&emsp;&emsp;In this comprehensive guide, I will walk you through the process of building this project, providing detailed instructions and insights along the way.

## 1. Clone

&emsp;&emsp;To get started, you can clone the repository using the `git clone` command:

```sh
git clone git@github.com:SwordofMorning/litelog.git
git clone https://github.com/SwordofMorning/litelog.git
```

&emsp;&emsp;Alternatively, you can download the zip file to your Linux computer and extract its contents using the `tar -xvf` command.

## 2. Prebuild

### 2.1 Cross-Compiler

&emsp;&emsp;Ensure that you have the appropriate cross-compiler installed. In this project, we utilize `gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu`, which is specified in the `./CMakeLists.txt` file:

```cmake
SET(CMAKE_C_COMPILER aarch64-none-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER aarch64-none-linux-gnu-c++)
```

&emsp;&emsp;Feel free to modify these settings to use your preferred compiler, such as `musl`. If desired, you can also remove these lines to default to the system compiler.

&emsp;&emsp;The choice of `aarch64-none-linux-gnu` is motivated by the goal of building a generic binary executable program for the ARM64 platform that adheres to the GNU/Linux standard.

### 2.2 CMake

&emsp;&emsp;Verify that you have `cmake` installed on your computer.

&emsp;&emsp;The project specifies the minimum required version of CMake using the following command:

```cmake
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
```

&emsp;&emsp;While compatibility with other versions has not been extensively tested, the project's simplicity should facilitate any necessary modifications with minimal effort.

### 2.3 Make

&emsp;&emsp;Please be aware that there may be potential bugs or compatibility issues related to the `make` utility. Exercise caution and report any encountered problems.

### 2.4 Output

&emsp;&emsp;The build process generates two output files:

1. `litelog`: The main program, located in the `bin` directory.
2. `litelog_test`: The unit test program, located in the `test` directory.

&emsp;&emsp;The names of these programs are defined in the `./CMakeLists.txt` file:

```cmake
SET(APP_NAME "litelog")
SET(TEST_NAME "litelog_test")
```

## 3. Building

&emsp;&emsp;For your convenience, a bash script `./build.sh` is provided to automate the entire build process. Alternatively, you can manually build the project using the `cmake` and `make` commands.