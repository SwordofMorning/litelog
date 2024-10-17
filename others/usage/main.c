#include "../include/litelog.h"

int main()
{
    litelog.init();

    char buffer[] = " Hello";
    for (int i = 0; i < 10; ++i)
    {
        buffer[0] = '0' + i;
        litelog.log.info(buffer);
    }

    sleep(1);
    litelog.change_level(LOG_LEVEL_D);
    for (int i = 0; i < 10; ++i)
    {
        litelog.log.debug("Hahaha %d", i);
    }

    sleep(1);
    // litelog.switch_page();
    litelog.change_level(LOG_LEVEL_D | LOG_LEVEL_E | LOG_LEVEL_I | LOG_LEVEL_W);
    for (int i = 0; i < 10; ++i)
    {
        litelog.log.details(LOG_LEVEL_D, __FILE__, __LINE__, __func__, "Hello? %d", i);
    }

    sleep(1);

    litelog.shutdown();

    litelog.exit();
}