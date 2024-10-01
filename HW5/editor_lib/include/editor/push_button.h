#pragma once

#include "controller.h"

class PushButton : public Controller
{
public:
    virtual void press(){
        activate();
    }
};