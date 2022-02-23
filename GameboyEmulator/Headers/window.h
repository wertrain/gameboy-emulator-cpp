#pragma once

#include <cinttypes>

class IWindow
{
public:
    struct CreateParameter {};

public:
    IWindow() {};
    virtual ~IWindow() {};

    virtual bool Create(const CreateParameter* param) { return false; }
    virtual void Run() {};
    virtual void Destroy() {};
};
