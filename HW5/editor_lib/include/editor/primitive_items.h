#pragma once

#include "graphics_item.h"

#include <iostream>

class GraphicsLineItem : public GraphicsItem
{
public:
    void Draw(Painter& painter) { std::cout << "Drawing line\n";}
};

class GraphicsRectItem : public GraphicsItem
{
public:
    void Draw(Painter& painter) { std::cout << "Drawing rect\n"; }
};

class GraphicsEllipseItem : public GraphicsItem
{
public:
    void Draw(Painter& painter) { std::cout << "Drawing ellipse\n"; }
};