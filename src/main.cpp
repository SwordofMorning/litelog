#include <iostream>
#include "utils/config/config.h"
#include "buffer/buffer.h"
#include "formatter/formatter.h"
#include "logger/clock.h"
#include "logger/logger.h"
#include "controller/controller.h"
#include "sink/sink_socket.h"
#include "sink/sink_kernel.h"

int main()
{
    /* --- Step 1 : Init ---*/

    Init();

    /* --- Step 2 : Create Source ---*/

    Clock::Start();
    Buffer buff(l1_cap, l2_cap);

    std::vector<std::unique_ptr<ISink>> sinks;
    sinks.push_back(std::make_unique<SocketSink>(listen_ip, listen_port));
    sinks.push_back(std::make_unique<KernelSink>());

    std::thread logger{Logger::Start(std::move(sinks), buff)};
    std::thread formatter{Formatter::Start(std::string{log_path} + std::string{log_prefix}, buff, log_lines)};

    /* --- Step 3 : Controller Listen in Main Thread ---*/

    Controller ctl(ctl_recv_ip, ctl_recv_port, ctl_send_ip, ctl_send_port, Logger::Get_Instance(), Formatter::Get_Instance());
    ctl();
    // wait one second for threadpool
    sleep(1);

    /* --- Step 4 : Release Source ---*/

    Logger::Stop();
    if (logger.joinable())
        logger.join();

    Formatter::Stop();
    if (formatter.joinable())
        formatter.join();

    Clock::Stop();

    return 0;
}