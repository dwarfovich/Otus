#pragma once

#include "controller.h"

class PushButton : public Controller
{
public:
    PushButton(const std::shared_ptr<Command>& command = nullptr): Controller{command}{}
    virtual void press(){
        activate();
    }
};