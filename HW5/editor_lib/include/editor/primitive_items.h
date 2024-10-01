#pragma once

#include "graphics_item.h"

#include <iostream>
#include <memory>

class GraphicsLineItem : public GraphicsItem
{
public:
    void Draw(Painter& painter) { std::cout << "Drawing line\n";}
    std::shared_ptr<GraphicsItem> clone() { return std::make_shared<GraphicsLineItem>(); }
};

class GraphicsRectItem : public GraphicsItem
{
public:
    void Draw(Painter& painter) { std::cout << "Drawing rect\n"; }
    std::shared_ptr<GraphicsItem> clone() { return std::make_shared<GraphicsRectItem>(); }
};

class GraphicsEllipseItem : public GraphicsItem
{
public:
    void Draw(Painter& painter) { std::cout << "Drawing ellipse\n"; }
    std::shared_ptr<GraphicsItem> clone() { return std::make_shared<GraphicsEllipseItem>(); }
};

class GraphicsTextItem : public GraphicsItem
{
public:
    void Draw(Painter& painter) { std::cout << "Drawing text\n"; }
    std::shared_ptr<GraphicsItem> clone() { return std::make_shared<GraphicsTextItem>(); }
};