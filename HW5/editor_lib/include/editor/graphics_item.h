#pragma once

#include "serializable.h"

#include <memory>

class Painter;

class GraphicsItem : public Serializeble
{
public:
    virtual int uuid() const noexcept = 0;
    virtual void Draw(Painter& painter) = 0;
    virtual std::shared_ptr<GraphicsItem> clone() = 0;
};