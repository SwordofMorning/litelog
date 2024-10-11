#pragma once

#include <pthread.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <stdarg.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <assert.h>  
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <string>
#include <fstream>
#include <mutex>
#include <ctime>
#include <iostream>

class Writer
{
private:
    std::ofstream m_log_file;
    std::mutex m_file_mutex;
public:
    Writer(const std::string& log_path);
    ~Writer();

    void Write(const std::string& str);
};