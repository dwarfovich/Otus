#pragma once

#include "command.h"
#include "document.h"

#include <memory>

class GraphicsItem;

class AddGraphicsItemCommand : public Command
{
public:
    AddGraphicsItemCommand(const std::shared_ptr<Document>&     document,
                           const std::shared_ptr<GraphicsItem>& item = nullptr)
        : document_ { document }, item_ { item }
    {
    }

    void execute() { document_->addItem(item_->clone()); }

private:
    std::shared_ptr<Document>     document_ = nullptr;
    std::shared_ptr<GraphicsItem> item_     = nullptr;
};