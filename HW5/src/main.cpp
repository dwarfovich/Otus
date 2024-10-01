#include "editor/document.h"
#include "editor/main_window.h"
#include "editor/view.h"
#include "editor/push_button.h"
#include "editor/add_graphics_item_command.h"
#include "editor/primitive_items.h"

#include <iostream>
#include <memory>
#include <vector>

int main()
{
    std::vector<std::shared_ptr<GraphicsItem>> items;
    auto       doc = std::make_shared<Document>();
    MainWindow mainWindow;
    mainWindow.addChild(std::make_shared<View>(doc));

    auto createAddGraphicsItemCommand = [&doc, &items]<typename GraphicsItemType>() {
        items.push_back(std::make_shared<GraphicsItemType>());
        return std::make_shared<AddGraphicsItemCommand>(doc, items.back());
    };

    auto addLineButton =
        std::make_shared<PushButton>(createAddGraphicsItemCommand.template operator()<GraphicsLineItem>());
    mainWindow.addChild(addLineButton);
    auto addRectButton =
        std::make_shared<PushButton>(createAddGraphicsItemCommand.template operator()<GraphicsRectItem>());
    mainWindow.addChild(addRectButton);
    auto addTextButton =
        std::make_shared<PushButton>(createAddGraphicsItemCommand.template operator()<GraphicsTextItem>());
    mainWindow.addChild(addTextButton);

    // Run commands.
    addLineButton->activate();
    addRectButton->activate();
    addTextButton->activate();

    doc->removeItem(items.back());


    return 0;
}
