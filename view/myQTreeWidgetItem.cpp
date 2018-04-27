//
// Created by michele on 05/04/17.
//

#include "myQTreeWidgetItem.h"
myQTreeWidgetItem::myQTreeWidgetItem(QTreeWidgetItem* parent, fileSystemNode* n) : QTreeWidgetItem(parent), node(n) {}
fileSystemNode* myQTreeWidgetItem::getNode() const {
    return node;
}
myQTreeWidgetItem::~myQTreeWidgetItem() {}