#include "socket_p.h"

Socket::Socket(const char* local_ip, const uint16_t& local_port, 
        const char* target_ip, const uint16_t& target_port)
{
    // Init local socket.
    Socket_Init(&m_local, local_ip, local_port);
    // Wrap remote ip and port.
    Socket_Create_Target(&m_remote, target_ip, target_port);
}

Socket::Socket(const std::string& local_ip, const uint16_t& local_port, 
        const std::string& target_ip, const uint16_t& target_port)
{
    // Init local socket.
    Socket_Init(&m_local, local_ip.c_str(), local_port);
    // Wrap remote ip and port.
    Socket_Create_Target(&m_remote, target_ip.c_str(), target_port);
}

Socket::~Socket()
{
    Socket_Exit(&m_local);
}

int Socket::Send(uint8_t* buffer, size_t n)
{
    return Socket_Send(m_local.device, buffer, n, (sockaddr*)&m_remote);
}

int Socket::Recv(uint8_t* buffer, size_t n, int timeout_ms)
{
    return Socket_Recv(m_local.device, buffer, n, (sockaddr*)&m_remote, timeout_ms);
}

Socket_Listen::Socket_Listen(const char* local_ip, const uint16_t& local_port)
{
    Socket_Init(&m_listen2, local_ip, local_port);
}

Socket_Listen::Socket_Listen(const std::string& local_ip, const uint16_t& local_port)
{
    Socket_Init(&m_listen2, local_ip.c_str(), local_port);
}

Socket_Listen::~Socket_Listen()
{
    Socket_Exit(&m_listen2);
}

int Socket_Listen::Recv(uint8_t* buffer, size_t n, int timeout_ms)
{
    struct sockaddr_in fake_addr;
    return Socket_Recv(m_listen2.device, buffer, n, (sockaddr*)&fake_addr, timeout_ms);
}