//
// Created by michele on 27/02/17.
//

#include "fileSystemNode.h"
#include "directoryNode.h"
fileSystemNode::fileSystemNode(const string& s) : name(s), parent(0) {}
fileSystemNode::fileSystemNode(const fileSystemNode& f) : name(f.name), parent(0) {}
fileSystemNode& fileSystemNode::operator=(const fileSystemNode& f) {
    if(&f != this){
        name = f.name;
        parent = 0;
    }
    return *this;
}
fileSystemNode* fileSystemNode::getParent() const{
    return parent;
}
string fileSystemNode::getName() const {
    return name;
}
//PRE = riceve in input un puntatore a nodo per settarne correttamente il genitore
int fileSystemNode::setParent(fileSystemNode* f) {
    directoryNode* newParent = dynamic_cast<directoryNode*>(f);
    if(f && !newParent){//verifico che il puntatore al padre non sia una directory, in tal caso sollevo eccezione
        throw feedback(0, "Attenzione, il genitore deve essere una directory");
    }
    directoryNode* oldParent = dynamic_cast<directoryNode*>(parent);//uso lo static cast perchè assumo parent puntatore upcastato a directoryNode
    parent = newParent;//modificando in anticipo il parent del nodo, così la funzione addNode si accprgererà che è stata chiamata da setParent
    // ed eviterà di chiamare setParent a sua volta
    int pos = -1;
    if(newParent && !newParent->existName(getName())){//controllo che il nuovo parent sia valido e non sia già presente nel nuovo genitore, se così fosse
                                             //vorrebbe dire che setParent è stata chiamata da addNode... in qiesto caso evito di richiamare AaddNode
        try {
            pos = newParent->addNode(this);
        }catch (feedback feed) {
            parent = oldParent;
            throw feed;
        }
    }
    //controllo che la funzione setParent non sia stata chiamata da removeNode, per farlo controllo che il genitore contenga ancora al suo interno tale nodo
    if(oldParent && oldParent->exist(this) && newParent != oldParent){//l'ultimo test serve per evitare che il nodo venga rimosso nel caso
                                                                      // si chiami 2 volte setParent con la stessa cartella
        oldParent->removeNode(this);
    }
    return pos;
}
/*POST = setta correttamente il parent del nodo su cui è invocata:
            -se viene invocata da utente sposta un nodo da una cartella ad un'altra (eventualmente sollevando eccezioni)
             utilizzando le chimate addNode sul nuovo genitore e removeNode sul vecchio genitore
            -se setParent viene invocata da addNode o removeNode si preoccupa solo di settare il parent, evitando di chiamare queste due funzioni
  */
bool operator<= (const fileSystemNode& f1, const fileSystemNode& f2){
    return f1.getName() <= f2.getName();
}
int fileSystemNode::rename(const string& newName){
    if(newName != name){
        if(parent){
            directoryNode* dir = dynamic_cast<directoryNode*>(parent);
            if(dir->existName(newName)){
                throw feedback(0, "Attenzione, il nome scelto è gia utilizzato nella directory corrente");
            }
            dir->removeNode(this);
            name = newName;
            return dir->addNode(this);
        }
        else{
            name = newName;
        }
    }
    return -1;
}
fileSystemNode::~fileSystemNode() {
    setParent(0);
}