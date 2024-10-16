#include "../include/litelog.h"

int main()
{
    // first ip:port is this program, second ip:port is which litelog listen to.
    litelog.init();

    char buffer[] = " Hello";
    for (int i = 0; i < 10; ++i)
    {
        buffer[0] = '0' + i;
        litelog.log(1 << i, buffer, sizeof(buffer));
    }

    litelog.shutdown();

    litelog.exit();
}