#pragma once

#include "../utils/config/config.h"
#include "../logger/message.h"
#include "../utils/global/global.h"

class ISink
{
public:
    virtual ~ISink() = default;
    virtual void Init() = 0;
    virtual void Exit() = 0;
    virtual bool Poll(RawMessage& raw_msg) = 0;
};