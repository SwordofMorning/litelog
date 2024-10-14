#include <iostream>
#include "buffer/buffer.h"
#include "buffer/writer.h"
#include "socket/monitor.h"

int main()
{
    Buffer buff(2000, 2000);
    std::thread monitor{Monitor::Start("127.0.0.1", 12345, buff)};
    std::thread writer{Writer::Start("/root/Unit", buff, 2000)};

    while (true)
    {
        // do nothing
    }

    Monitor::Stop();
    monitor.join();
    Writer::Stop();
    writer.join();
}