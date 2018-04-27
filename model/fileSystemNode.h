//
// Created by michele on 27/02/17.
//

#ifndef CLOUD_2_0_FILESYSTEMNODE_H
#define CLOUD_2_0_FILESYSTEMNODE_H

#include <string>
#include "byteSize.h"
using  std::string;

class fileSystemNode {
    friend bool operator<= (const fileSystemNode&, const fileSystemNode&);
private:
    string name;
    fileSystemNode* parent;//assumo che sia un puntatore a directoryNode, inserendo un controllo nel meotodo setParent
public:
    fileSystemNode(const string&);
    fileSystemNode(const fileSystemNode&);
    fileSystemNode& operator= (const fileSystemNode&);
    fileSystemNode* getParent() const;
    string getName() const;
    int setParent(fileSystemNode*);
    int rename(const string&);
    virtual fileSystemNode* clone(fileSystemNode* = 0) const = 0;
    virtual byteSize getSize() const = 0;
    virtual string getInformation() const = 0;
    virtual string getType() const = 0;
    virtual bool contains(fileSystemNode *) const = 0;
    virtual ~fileSystemNode();
};


#endif //CLOUD_2_0_FILESYSTEMNODE_H
