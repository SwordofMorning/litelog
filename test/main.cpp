#include "../utils/version.h"
#include "../src/config/config.h"
#include "./case/socket/socket.hpp"

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
    printf("[----------] Global test environment set-up.\n");

    RUN_TEST_CASE(socket_p2p_unidirectional);
    RUN_TEST_CASE(socket_p2p_bidirectional);

    printf("[----------] Global test environment tear-down.\n");
    print_test_results();

    return 0;
}