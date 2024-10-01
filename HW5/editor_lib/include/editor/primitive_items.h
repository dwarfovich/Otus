#pragma once

#include "graphics_item.h"

#include <iostream>
#include <memory>

class GraphicsLineItem : public GraphicsItem
{
public:
    void                          read(std::istream& stream) override { std::cout << "Reading line\n"; }
    void                          write(std::ostream& stream) const override { std::cout << "Writing line\n"; }
    int                           uuid() const noexcept override { return 1; }
    void                          Draw(Painter& painter) { std::cout << "Drawing line\n"; }

    std::shared_ptr<GraphicsItem> clone() { return std::make_shared<GraphicsLineItem>(); }
};

class GraphicsRectItem : public GraphicsItem
{
public:
    void                          read(std::istream& stream) override { std::cout << "Reading rect\n"; }
    void                          write(std::ostream& stream) const override { std::cout << "Writing rect\n"; }
    int                           uuid() const noexcept override { return 2; }
    void                          Draw(Painter& painter) { std::cout << "Drawing rect\n"; }
    std::shared_ptr<GraphicsItem> clone() { return std::make_shared<GraphicsRectItem>(); }
};

class GraphicsEllipseItem : public GraphicsItem
{
public:
    void                          read(std::istream& stream) override { std::cout << "Reading ellipse\n"; }
    void                          write(std::ostream& stream) const override { std::cout << "Writing ellipse\n"; }
    int                           uuid() const noexcept override { return 3; }
    void                          Draw(Painter& painter) { std::cout << "Drawing ellipse\n"; }
    std::shared_ptr<GraphicsItem> clone() { return std::make_shared<GraphicsEllipseItem>(); }
};

class GraphicsTextItem : public GraphicsItem
{
public:
    void                          read(std::istream& stream) override { std::cout << "Reading text item\n"; }
    void                          write(std::ostream& stream) const override { std::cout << "Writing text item\n"; }
    int                           uuid() const noexcept override { return 4; }
    void                          Draw(Painter& painter) { std::cout << "Drawing text\n"; }
    std::shared_ptr<GraphicsItem> clone() { return std::make_shared<GraphicsTextItem>(); }
};