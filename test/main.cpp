#include "../utils/version.h"
#include "env/global.hpp"
#include "../src/config/config.h"
#include "./case/socket/socket.hpp"
#include "./case/socket/monitor.hpp"
#include "./case/buffer/writer.hpp"

void Version()
{
    printf("[==========] Version Information.\n");
    printf("[  Branch  ] %s\n", __GIT_BRANCH__);
    printf("[   User   ] %s\n", __GIT_USER__);
    printf("[  Commit  ] %s\n", __GIT_COMMIT_ID__);
}

int main()
{
    Version();

    printf("[==========] Test Begin.\n");
    std::thread monitor{Monitor::Start("127.0.0.1", 12345, buf)};
    printf("[----------] Global test environment set-up.\n");

    RUN_TEST_CASE(socket_unidirectional);
    RUN_TEST_CASE(socket_bidirectional);
    RUN_TEST_CASE(socket_listen);
    RUN_TEST_CASE(monitor_and_buf);
    RUN_TEST_CASE(writer_write_from_buffer);

    printf("[----------] Global test environment tear-down.\n");
    Monitor::Stop();
    monitor.join();
    print_test_results();

    return 0;
}