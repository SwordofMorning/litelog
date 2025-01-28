# Litelog

![Code Style](https://github.com/SwordofMorning/litelog/workflows/Code_Style_Check/badge.svg)
![Build](https://github.com/SwordofMorning/litelog/workflows/Build_Check/badge.svg)

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
│   ├── ini                     # ini file pattern.
│   ├── py                      # python scripts, used to extract log file from target machine.
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
├── test                    # unit test.
└── usag                    # example for others program that wish to use litelog.
```

## II. Build

See reference on <a href = "./docs/wiki/build.md">Build</a>.

## III. Usage

See reference on <a href = "./docs/wiki/usage.md">Usage</a>.

### IV. VSC Plug

<a href="https://github.com/SwordofMorning/litelog-highlighter">litelog-highlighter</a> is a VS Code plug, which is used to highlight the log lines. You could just search and install it in VSC.