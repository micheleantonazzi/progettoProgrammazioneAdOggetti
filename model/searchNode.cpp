//
// Created by michele on 10/05/17.
//

#include "searchNode.h"
searchNode::searchNode(const string& r, bool f, bool d, const byteSize& min, const byteSize& max) :
        reg(r, std::regex::ECMAScript), file(f), directory(d), minSize(min), maxSize(max), empty(false){
    if(!r.length()){
        empty = true;
    }
}
bool searchNode::isEmpty() const {
    return empty;
}
regex searchNode::getReg() const{
    return reg;
}
bool searchNode::getFile() const{
    return file;
}
bool searchNode::getDirectory() const{
    return directory;
}
byteSize searchNode::getMinSize() const{
    return minSize;
}
byteSize searchNode::getMaxSize() const{
    return maxSize;
}