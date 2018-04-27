//
// Created by michele on 27/02/17.
//

#ifndef CLOUD_2_0_DIRECTORYNODE_H
#define CLOUD_2_0_DIRECTORYNODE_H

#include "fileSystemNode.h"
#include <iostream>
#include <list>
#include "feedback.h"
using std::list;
class directoryNode : public virtual fileSystemNode{
private:
    list<fileSystemNode*> subNodes;
    static void copy(const list<fileSystemNode*>&, fileSystemNode *parent);
    static bool comparePointer (fileSystemNode*, fileSystemNode*);// usata per ordinare i nodi all'interno di una cartella
    void drop();
public:
    directoryNode(const string& = "directory unknown");
    directoryNode(const directoryNode&);
    directoryNode& operator= (const directoryNode&);
    list<fileSystemNode*> openDirectory() const;
    std::pair<int, int> countNodes() const; //indica quante sottocartelle e file sono contenuti in questa directory
    virtual directoryNode* clone(fileSystemNode* = 0) const;
    virtual byteSize getSize() const;
    virtual string getInformation() const;
    virtual string getType() const;
    int addNode(fileSystemNode*); //solleva eccezione se il puntatore è nullo, se il puntatore è uguale alla cartella stessa e se il nodo è gia presente
    int removeNode(fileSystemNode*);//solleva eccezione se la cartella è vuota o se il file non è presente
    bool exist(fileSystemNode*) const;
    bool existName(const string&) const;
    virtual bool contains(fileSystemNode *) const;
    virtual ~directoryNode();
};


#endif //CLOUD_2_0_DIRECTORYNODE_H
