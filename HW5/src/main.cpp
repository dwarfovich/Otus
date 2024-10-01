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
    auto       doc = std::make_shared<Document>();
    MainWindow mainWindow;
    mainWindow.addChild(std::make_shared<View>(doc));

    auto createGraphicsItemCommand = [&doc]<typename GraphicsItemType>() {
        return std::make_shared<AddGraphicsItemCommand>(doc, std::make_shared<GraphicsItemType>());
    };

    auto addLineButton =
        std::make_shared<PushButton>(createGraphicsItemCommand.template operator()<GraphicsLineItem>());
    mainWindow.addChild(addLineButton);
    auto addRectButton =
        std::make_shared<PushButton>(createGraphicsItemCommand.template operator()<GraphicsRectItem>());
    mainWindow.addChild(addRectButton);
    auto addTextButton =
        std::make_shared<PushButton>(createGraphicsItemCommand.template operator()<GraphicsTextItem>());
    mainWindow.addChild(addTextButton);

    addLineButton->activate();
    addRectButton->activate();
    addTextButton->activate();

    return 0;
}
