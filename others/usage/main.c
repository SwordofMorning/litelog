#include "../include/litelog.h"

int main()
{
    // first ip:port is this program, second ip:port is which litelog listen to.
    Litelog_Init("127.0.0.1", 9999, "127.0.0.1", 12345);

    uint8_t buffer[5] = {0x01, 'A', 'B', 'C', ' '};
    for (int i = 0; i < 10; ++i)
    {
        buffer[4] = '0' + i;
        Litelog_Send(buffer, 5);
    }

    // which is remote of controller
    struct Socket_Wrap lo;
    Socket_Init(&lo, "127.0.0.1", 12347);
    // which is local of controller
    struct sockaddr_in re;
    Socket_Create_Target(&re, "127.0.0.1", 12346);
    // close
    uint8_t command[1] = {0x00};
    Socket_Send(lo.device, command, 1, (struct sockaddr*)&re);

    Litlog_Exit();
}