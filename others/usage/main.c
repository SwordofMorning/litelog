#include "../include/litelog.h"

int main()
{
    Litelog_Init("127.0.0.1", 9999, "127.0.0.1", 12345);

    uint8_t buffer[5] = {0x01, 'A', 'B', 'C', ' '};
    for (int i = 0; i < 10; ++i)
    {
        buffer[4] = '0' + i;
        Litelog_Send(buffer, 5);
    }

    Litlog_Exit();
}