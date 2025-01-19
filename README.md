# Litelog

![Code Style Check](https://github.com/SwordofMorning/Litelog/workflows/Code_Style_Check/badge.svg)
![Build Check](https://github.com/SwordofMorning/Litelog/workflows/Build_Check/badge.svg)

## I. Architecture

This project is built using cmake. For ease of use, `build.sh` is written to quickly call `cmake ..` and `make` commands. The file structure of the project is as follows:

```sh
.
├── build.sh                # compile bash script.
├── .clang-format           # clang-format config.
├── CMakeLists.txt          # project cmake file.
├── docs
├── .github                 # github workflow.
├── .gitignore
├── LICENSE
├── others                  # usage and some scripts.
│   ├── include                 # That's all you need if your program needs to call litelog.
│   ├── ini                     # ini file pattern.
│   ├── py                      # python scripts, used to extract log file from target machine.
│   ├── usage                   # usage example.
│   └── version                 # version control information.
├── README.md
├── src                     # resource.
│   ├── buffer                  # log buffer.
│   ├── CMakeLists.txt          # cmake for litelog itself.
│   ├── controller              # a controller which could set litelog's action via socket.
│   ├── formatter               # write log from buffer into file.
│   ├── logger                  # move log from sink into buffer.
│   ├── main.cpp                # main function.
│   ├── sink                    # move log from kernel or socket into logger.
│   └── utils                   # some tools.
│       ├── config                  # init function, and some enum.
│       ├── date                    # get system time.
│       ├── global                  # global variable.
│       ├── socket                  # socket interface.
│       └── threadpool              # threadpool for logger.
└── test                    # unit test.
```

## II. Build

See reference on <a href = "./docs/wiki/build.md">Build</a>.

## III. Usage

TODO