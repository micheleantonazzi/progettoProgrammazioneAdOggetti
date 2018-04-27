//
// Created by michele on 08/03/17.
//

#ifndef CLOUD_2_0_DIRECTORYNODESHARED_H
#define CLOUD_2_0_DIRECTORYNODESHARED_H

#include "directoryNode.h"
#include "fileSystemNodeShared.h"

class directoryNodeShared : public directoryNode, public fileSystemNodeShared{
private:
public:
    directoryNodeShared(const directoryNode&, const string&);
    directoryNodeShared(directoryNode*, const string&);
    virtual string getInformation() const;
    virtual directoryNodeShared* clone(fileSystemNode* = 0) const;
    virtual string getType() const;
};


#endif //CLOUD_2_0_DIRECTORYNODESHARED_H
