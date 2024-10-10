#include "../src/config/config.h"
#include "./case/socket/socket.hpp"

int main()
{
    printf("[==========] Test Begin.\n");
    printf("[----------] Global test environment set-up.\n");

    RUN_TEST_CASE(socket_send_recv);

    printf("[----------] Global test environment tear-down.\n");
    print_test_results();

    return 0;
}