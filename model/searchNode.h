//
// Created by michele on 10/05/17.
//

#ifndef CLOUD_2_0_SEARCHNODE_H
#define CLOUD_2_0_SEARCHNODE_H

#include "string"
#include "byteSize.h"
#include <regex>
using std::regex;
using std::string;
class searchNode {
private:
    regex reg;
    bool file;
    bool directory;
    byteSize minSize;
    byteSize maxSize;
    bool empty;
public:
    searchNode(const string&, bool, bool, const byteSize&, const byteSize&);
    regex getReg() const;
    bool getFile() const;
    bool getDirectory() const;
    byteSize getMinSize() const;
    byteSize getMaxSize() const;
    bool isEmpty() const;
};


#endif //CLOUD_2_0_SEARCHNODE_H
