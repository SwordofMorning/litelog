#pragma once

#include "sink.h"
#include "../utils/socket/socket_p.h"

class SocketSink : public ISink
{
private:
    Socket_Listen m_listen;

    static constexpr size_t BUFFER_SIZE = 100;
    uint8_t m_recv_buffer[BUFFER_SIZE];

public:
    SocketSink(const char* listen_ip, const uint16_t& listen_port);

    virtual void Init() override;

    virtual void Exit() override;

    virtual bool Poll(RawMessage& raw_msg) override;
};