# Litelog 日志程序

![Code Style Check](https://github.com/SwordofMorning/Litelog/workflows/Code_Style_Check/badge.svg)
![Build Check](https://github.com/SwordofMorning/Litelog/workflows/Build_Check/badge.svg)

## 一、工程结构

本项目使用cmake构建，为了方便使用而编写了`build.sh`用于快速调用`cmake ..`和`make`命令。项目的文件结构如下：

```sh
.
├── build.sh                                # 编译脚本
├── CMakeLists.txt                          # 项目整体的cmake
├── LICENSE                                 # LICENSE
├── others                                  # 其他文件
│   ├── include                                 # 以源码形式被其他程序调用的API接口
│   │   ├── litelog.c
│   │   └── litelog.h
│   ├── ini                                     # 配置文件
│   │   └── litelog.ini
│   ├── usage                                   # 参考用例
│   │   ├── CMakeLists.txt
│   │   └── main.c
│   └── version                                 # 版本信息
│       ├── version.h
│       └── version.h.in
├── README.md                               # README
├── src                                     # source目录
│   ├── buffer                                  # 缓冲区设计
│   │   ├── buffer.cpp
│   │   └── buffer.h
│   ├── CMakeLists.txt                          # src下的cmake
│   ├── controller                              # 用于监听或控制litelog自身的类
│   │   ├── controller.cpp
│   │   ├── controller.h
│   │   ├── file_maintainer.cpp
│   │   └── file_maintainer.h
│   ├── formatter                               # 日志写入文件
│   │   ├── formatter.cpp
│   │   └── formatter.h
│   ├── logger                                  # 日志记录、编排
│   │   ├── logger.cpp
│   │   ├── logger.h
│   │   ├── message.cpp
│   │   └── message.h
│   ├── main.cpp                                # 主函数
│   ├── sink                                    # 日志接收器
│   │   ├── sink.cpp
│   │   ├── sink.h
│   │   ├── sink_kernel.cpp                         # 系统日志
│   │   ├── sink_kernel.h
│   │   ├── sink_socket.cpp                         # 应用日志
│   │   └── sink_socket.h
│   └── utils                               # 工具类
│       ├── config                              # 初始化和define
│       ├── date                                # 时间工具
│       ├── global                              # 全局变量
│       ├── socket                              # socket接口和封装
│       └── threadpool                          # 线程池
└── test
    ├── case
    │   ├── buffer
    │   └── socket
    ├── CMakeLists.txt
    ├── env
    │   └── global.hpp
    ├── main.cpp
    └── utils
        ├── utils.c
        └── utils.h
```