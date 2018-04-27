//
// Created by michele on 26/04/17.
//

#include "myQTableWidgetItem.h"
myQTableWidgetItem::myQTableWidgetItem(fileSystemNode* n) : QTableWidgetItem(), node(n){}
fileSystemNode* myQTableWidgetItem::getNodePointer() const {
    return node;
}
myQTableWidgetItem::~myQTableWidgetItem() {}