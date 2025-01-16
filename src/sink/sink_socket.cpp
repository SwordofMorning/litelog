#include "sink_socket.h"

SocketSink::SocketSink(const char* listen_ip, const uint16_t& listen_port)
    : m_listen(listen_ip, listen_port)
{
    // do nothing
}

void SocketSink::Init()
{
    // do nothing
}

void SocketSink::Exit()
{
    // do nothing
}

bool SocketSink::Poll(RawMessage& raw_msg)
{
    int ret = m_listen.Recv(m_recv_buffer, BUFFER_SIZE, 100);
    if (ret <= 0)
        return false;

    raw_msg.level = m_recv_buffer[0];
    raw_msg.content.assign(reinterpret_cast<char*>(m_recv_buffer + 1), ret - 1);
    return true;
}