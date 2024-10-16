#include <iostream>
#include "config/config.h"
#include "buffer/buffer.h"
#include "buffer/writer.h"
#include "listen/monitor.h"
#include "listen/controller.h"

int main()
{
    Init();

    Buffer buff(l1_cap, l2_cap);
    std::thread monitor{Monitor::Start(listen_ip, listen_port, buff)};
    std::thread writer{Writer::Start(std::string{log_path} + std::string{log_prefix}, buff, log_lines)};

    Monitor& m = Monitor::Get_Instance();
    Writer& w = Writer::Get_Instance();

    Controller ctl("127.0.0.1", 20000, "127.0.0.1", 30000, m, w);

    ctl();
    sleep(1);

    Monitor::Stop();
    monitor.join();
    Writer::Stop();
    writer.join();
}