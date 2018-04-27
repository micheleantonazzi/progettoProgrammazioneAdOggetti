//
// Created by michele on 27/02/17.
//

#include "fileNodeShared.h"
fileNodeShared::fileNodeShared(const fileNode& f, const string& s) : fileSystemNode(f), fileNode(f), fileSystemNodeShared(f.getName(), s) {}
fileNodeShared::fileNodeShared(fileNode* f, const string& s) : fileSystemNode(*f), fileNode(*f), fileSystemNodeShared(f->getName(), s){}
string fileNodeShared::getInformation() const {
    return fileNode::getInformation() + "\n" + "File condiviso da: " + sharedBy();
}
fileNodeShared* fileNodeShared::clone(fileSystemNode *p) const{
    fileNodeShared* copy = new fileNodeShared(*this);
    copy->setParent(p);
    return copy;
}
string fileNodeShared::getType() const {
    return "fileNodeShared";
}