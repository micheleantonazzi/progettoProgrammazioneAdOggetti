//
// Created by michele on 08/03/17.
//

#include "rootNode.h"
rootNode::rootNode(const string& email, const string &name) : fileSystemNode(email), directoryNode(), userName(name) {}
rootNode* rootNode::clone(fileSystemNode *parent) const{
    rootNode *copy = new rootNode(*this);
    copy->setParent(parent);
    return copy;
}
string rootNode::getUserName() const {
    return userName;
}
void rootNode::changeUserName(const string& s) {
    userName = s;
}
string rootNode::getType() const {
    return "rootNode";
}