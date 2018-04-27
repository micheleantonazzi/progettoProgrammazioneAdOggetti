//
// Created by michele on 27/02/17.
//

#include "fileNode.h"
fileNode::fileNode() : fileSystemNode("unknown"){}
fileNode::fileNode(const string& s, const byteSize& b) : fileSystemNode(s), size(b) {}
fileNode::fileNode(const string& n, unsigned long long int num, const string& s) : fileSystemNode(n), size(num, s) {}
byteSize fileNode::getSize() const {
    return size;
}
string fileNode::getInformation() const {
    return "Nome: " + getName() +"\n" + "Dimensione: " + size.getSize();
}
fileNode* fileNode::clone(fileSystemNode* p) const{
    fileNode* copy = new fileNode(*this);
    copy->setParent(p);
    return copy;
}
bool fileNode::contains(fileSystemNode *) const {
    return false;
}
string fileNode::getType() const {
    return "fileNode";
}
ostream& operator<< (ostream& os, const fileNode& f){
    return os << f.getName();
}
fileNode::~fileNode() {}