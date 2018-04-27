//
// Created by michele on 27/02/17.
//

#ifndef CLOUD_2_0_FILENODESHARED_H
#define CLOUD_2_0_FILENODESHARED_H

#include "fileNode.h"
#include "fileSystemNodeShared.h"

class fileNodeShared : public fileNode, public fileSystemNodeShared{
private:
public:
    fileNodeShared(const fileNode&, const string&);//la costruzione di un file condiviso avviene solo in presenza di un file normale (non condiviso)
    fileNodeShared(fileNode*, const string&);
    virtual string getInformation() const;
    virtual fileNodeShared* clone(fileSystemNode* = 0) const;
    virtual string getType() const;
};


#endif //CLOUD_2_0_FILENODESHARED_H
