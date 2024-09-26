#pragma once

class Painter;

class GraphicsItem
{
public:
    virtual void Draw(Painter& painter) = 0;
};