#pragma once

#include <memory>

class Painter;

class GraphicsItem
{
public:
    virtual void Draw(Painter& painter) = 0;
    virtual std::shared_ptr<GraphicsItem> clone() = 0;
};