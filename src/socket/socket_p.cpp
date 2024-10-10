#include "socket_p.h"

Socket_P2P::Socket_P2P(const char* local_ip, const uint16_t& local_port, 
        const char* target_ip, const uint16_t& target_port)
{
    // Init local socket.
    Socket_Init(&m_local, local_ip, local_port);
    // Wrap remote ip and port.
    Socket_Create_Target(&m_remote, target_ip, target_port);
}

Socket_P2P::Socket_P2P(const std::string& local_ip, const uint16_t& local_port, 
        const std::string& target_ip, const uint16_t& target_port)
{
    // Init local socket.
    Socket_Init(&m_local, local_ip.c_str(), local_port);
    // Wrap remote ip and port.
    Socket_Create_Target(&m_remote, target_ip.c_str(), target_port);
}

Socket_P2P::~Socket_P2P()
{
    Socket_Exit(&m_local);
}

int Socket_P2P::Send(uint8_t* buffer, size_t n)
{
    return Socket_Send(m_local.device, buffer, n, (sockaddr*)&m_remote);
}

int Socket_P2P::Get(uint8_t* buffer, size_t n, int timeout_ms)
{
    return Socket_Get(m_local.device, buffer, n, (sockaddr*)&m_remote, timeout_ms);
}