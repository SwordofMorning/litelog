#include <iostream>
#include "buffer/buffer.h"
#include "buffer/writer.h"
#include "listen/monitor.h"
#include "listen/controller.h"

int main()
{
    // Buffer buff(2000, 2000);
    // std::thread monitor{Monitor::Start("127.0.0.1", 12345, buff)};
    // std::thread writer{Writer::Start("/root/Unit", buff, 2000)};

    // Monitor& m = Monitor::Get_Instance();
    // Writer& w = Writer::Get_Instance();

    // Controller ctl("127.0.0.1", 20000, "127.0.0.1", 20002, m, w);

    // while (true)
    // {
    //     ctl();
    // }

    // Monitor::Stop();
    // monitor.join();
    // Writer::Stop();
    // writer.join();
}