//
// Created by michele on 27/02/17.
//

#ifndef CLOUD_2_0_FILENODE_H
#define CLOUD_2_0_FILENODE_H

#include "fileSystemNode.h"
#include "byteSize.h"
#include "directoryNode.h"

class fileNode : public virtual fileSystemNode{
    friend ostream& operator<< (ostream&, const fileNode&);
private:
    byteSize size;
public:
    fileNode();
    fileNode(const string&, const byteSize& = byteSize());
    fileNode(const string&, unsigned long long int, const string& = "");
    virtual fileNode* clone(fileSystemNode* = 0) const;
    virtual byteSize getSize() const;
    virtual string getInformation() const;
    virtual string getType() const;
    virtual bool contains(fileSystemNode *) const;
    virtual ~fileNode();
};


#endif //CLOUD_2_0_FILENODE_H
