//
// Created by michele on 26/05/17.
//

#include "fileSystemNodeShared.h"
fileSystemNodeShared::fileSystemNodeShared(const string& name, const string& s) : fileSystemNode(name), shared(s) {}
string fileSystemNodeShared::sharedBy() const {
    return shared;
}
fileSystemNodeShared::~fileSystemNodeShared() {}