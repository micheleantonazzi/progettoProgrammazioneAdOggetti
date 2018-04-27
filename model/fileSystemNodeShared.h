//
// Created by michele on 26/05/17.
//

#ifndef CLOUD_2_0_FILESYSTEMNODESHARED_H
#define CLOUD_2_0_FILESYSTEMNODESHARED_H


#include "fileSystemNode.h"

class fileSystemNodeShared : public virtual fileSystemNode{
private:
    string shared;
public:
    fileSystemNodeShared(const string&, const string&);
    string sharedBy() const;
    virtual fileSystemNodeShared* clone(fileSystemNode* = 0) const = 0;
    virtual ~fileSystemNodeShared();
};


#endif //CLOUD_2_0_FILESYSTEMNODESHARED_H
