#include "../include/litelog.h"

int main()
{
    litelog.init("Demo_Program");
    // litelog.init("Demo_Program_With_Very_Very_Long_Name");

    litelog.log.fatal("Fatal!");
    litelog.log.error("Error!");
    litelog.log.warning("Warning!");
    litelog.log.notice("Notice!");
    litelog.log.info("Information!");
    litelog.log.debug("Debug!");
    litelog.log.trace("Trace!");

    sleep(1);

    litelog.change_level(LOG_LEVEL_D | LOG_LEVEL_F);
    // litelog.switch_page();
    for (int i = 0; i < 10; ++i)
    {
        printf("Send2: %d\n", 1 << i);
        litelog.log.manual(1 << i, __FILE__, __LINE__, __func__, "Send 2: %d", i);
    }

    litelog.log.manual(LOG_LEVEL_D | LOG_LEVEL_E, __FILE__, __LINE__, __func__, "Send 3");

    sleep(1);

    litelog.shutdown();

    litelog.exit();
}