//
// Created by michele on 08/03/17.
//

#ifndef CLOUD_2_0_ROOTNODE_H
#define CLOUD_2_0_ROOTNODE_H

#include "directoryNode.h"
class rootNode : public directoryNode{
private:
    string userName;
public:
    rootNode(const string&, const string&);//il nome della root è la mail dell'utente che la possiede
    void changeUserName(const string&);
    string getUserName() const;
    virtual rootNode* clone(fileSystemNode* = 0) const;
    virtual string getType() const;//usato per xml
};

#endif //CLOUD_2_0_ROOTNODE_H
