#pragma once

#include "command.h"
#include "document.h"

#include <memory>

class GraphicsItem;

class RemoveGraphicsItemCommand : public Command
{
public:
    RemoveGraphicsItemCommand(const std::shared_ptr<Document>&     document,
                           const std::shared_ptr<GraphicsItem>& item = nullptr)
        : document_ { document }, item_ { item }
    {
    }

    void execute() { document_->removeItem(item_); }

private:
    std::shared_ptr<Document>     document_ = nullptr;
    std::shared_ptr<GraphicsItem> item_     = nullptr;
};
