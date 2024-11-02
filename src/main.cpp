#include <iostream>
#include "utils/config/config.h"
#include "buffer/buffer.h"
#include "formatter/writer.h"
#include "logger/logger.h"
#include "controller/controller.h"

int main()
{
    Init();

    Buffer buff(l1_cap, l2_cap);
    std::thread logger{Logger::Start(listen_ip, listen_port, buff)};
    std::thread writer{Writer::Start(std::string{log_path} + std::string{log_prefix}, buff, log_lines)};

    Logger& m = Logger::Get_Instance();
    Writer& w = Writer::Get_Instance();

    Controller ctl(ctl_recv_ip, ctl_recv_port, ctl_send_ip, ctl_send_port, m, w);

    ctl();
    sleep(1);

    Logger::Stop();
    logger.join();
    Writer::Stop();
    writer.join();
}