#include <iostream>
#include "utils/config/config.h"
#include "buffer/buffer.h"
#include "formatter/formatter.h"
#include "logger/clock.h"
#include "logger/logger.h"
#include "controller/controller.h"
#include "sink/sink_socket.h"

int main()
{
    Init();

    Clock::Start();
    Buffer buff(l1_cap, l2_cap);

    auto socket_sink = std::make_unique<SocketSink>("127.0.0.1", 20000);

    std::thread logger{Logger::Start(std::move(socket_sink), buff)};
    std::thread formatter{Formatter::Start(std::string{log_path} + std::string{log_prefix}, buff, log_lines)};

    Logger& m = Logger::Get_Instance();
    Formatter& w = Formatter::Get_Instance();

    Controller ctl(ctl_recv_ip, ctl_recv_port, ctl_send_ip, ctl_send_port, m, w);

    ctl();
    sleep(1);

    Logger::Stop();
    logger.join();
    Formatter::Stop();
    formatter.join();
    Clock::Stop();

    return 0;
}