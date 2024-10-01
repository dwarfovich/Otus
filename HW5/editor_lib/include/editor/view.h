#pragma once

#include "widget.h"

#include <memory>

class Document;

class View : public Widget
{
public:
    virtual void update() = 0;
    virtual void setDocument(const std::shared_ptr<Document>& aDocument) { document = aDocument; }

protected:
    std::shared_ptr<Document> document;
};