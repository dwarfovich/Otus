#pragma once

#include "widget.h"

#include <memory>
#include <functional>
#include <iostream>

class Document;

class View : public Widget
{
public:
    View() = default;
    View(const std::shared_ptr<Document>& document) { setDocument(document); }
    virtual void update() {}
    virtual void setDocument(const std::shared_ptr<Document>& document)
    {
        document_ = document;
        using namespace std::placeholders;
        document_->addObserver(std::bind(&View::onDocumentChanged, this, _1, _2));
    }

protected: // methods
    virtual void onDocumentChanged(Document::ItemChangeType changeType, const GraphicsItem* item)
    {
        switch (changeType) {
            case Document::ItemChangeType::Added: std::cout << "New item added\n"; break;
            case Document::ItemChangeType::Removed: std::cout << "Item added\n"; break;
            case Document::ItemChangeType::Modified: std::cout << "Item modified\n"; break;
        }
    }

protected: // data
    std::shared_ptr<Document> document_ = nullptr;
};