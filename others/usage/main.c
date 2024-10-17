#include "../include/litelog.h"

int main()
{
    // first ip:port is this program, second ip:port is which litelog listen to.
    litelog.init();

    char buffer[] = " Hello";
    for (int i = 0; i < 10; ++i)
    {
        buffer[0] = '0' + i;
        // litelog.log(1 << i, buffer, sizeof(buffer));
        litelog.log.info(buffer);
    }

    sleep(1);
    litelog.change_level(LOG_LEVEL_D);
    for (int i = 0; i < 10; ++i)
    {
        buffer[0] = 'a' + i;
        // litelog.log(1 << i, buffer, sizeof(buffer));
        litelog.log.debug("Hahaha %d", i);
    }

    sleep(1);
    litelog.switch_page();
    litelog.change_level(LOG_LEVEL_D | LOG_LEVEL_E | LOG_LEVEL_I | LOG_LEVEL_W);
    for (int i = 0; i < 10; ++i)
    {
        buffer[0] = 'A' + i;
        // litelog.log(1 << i, buffer, sizeof(buffer));
    }

    litelog.shutdown();

    litelog.exit();
}