//
// Created by michele on 08/03/17.
//

#include "directoryNodeShared.h"
directoryNodeShared::directoryNodeShared(const directoryNode& d, const string& s) : fileSystemNode(d), directoryNode(d), fileSystemNodeShared(d.getName(), s) {}
directoryNodeShared::directoryNodeShared(directoryNode* d, const string& s) : fileSystemNode(*d), directoryNode(*d), fileSystemNodeShared(d->getName(), s) {}
string directoryNodeShared::getInformation() const {
    return "Cartella condivisa da: " + sharedBy() + "\n" + directoryNode::getInformation();
}
directoryNodeShared* directoryNodeShared::clone(fileSystemNode *parent) const{
    directoryNodeShared *copy = new directoryNodeShared(*this);
    copy->setParent(parent);
    return copy;
}
string directoryNodeShared::getType() const {
    return "directoryNodeShared";
}
