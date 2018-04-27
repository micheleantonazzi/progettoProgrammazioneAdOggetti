//
// Created by michele on 27/02/17.
//

#include "directoryNode.h"
#include "fileNode.h"
directoryNode::directoryNode(const string& name) : fileSystemNode(name){}
directoryNode::directoryNode(const directoryNode& dir) : fileSystemNode(dir), subNodes(){
    copy(dir.subNodes, this);
}
directoryNode& directoryNode::operator= (const directoryNode& dir){
    fileSystemNode::operator= (dir);
    drop();
    copy(dir.subNodes, this);
    return *this;
}
byteSize directoryNode::getSize() const {
    byteSize s;
    for (auto it = subNodes.begin(); it != subNodes.end(); ++it) {
        s += (*it)->getSize();
    }
    return s;
}
//PRE = accetta un nodo valido (anche nullo) e un std::pair che contiene il numero di file e cartelle per l'iterazione precedente
std::pair<int, int> directoryNode::countNodes() const{
    std::pair<int, int> p = std::make_pair(0, 0);
    for (auto it = subNodes.begin(); it != subNodes.end() ; ++it) {
        directoryNode* dir = dynamic_cast<directoryNode*>(*it);
        if(dir){
            std::pair<int, int> ret = dir->countNodes();
            p.first = ret.first;
            p.second = ret.second;
        }
        else{
            p.second++;
        }

    }
    p.first++;
    return p;
}
//POST = caso base: se il nodo è un file restituisce un pair contenente <0, 1>
//       caso induttivo: se il nodo è una cartella restituisce un pair <x1m, x2> dove x1 = numero di sottocartelle && x2 = numero di file
string directoryNode::getInformation() const {
    std::pair<int, int> num = countNodes();
    return "Cartella: " + getName() +
           "\nDimensione: " + directoryNode::getSize().byteSize::getSize() +
           "\nSottocartelle: " + std::to_string(--num.first) +
           "\nFile: " + std::to_string(num.second);
}
bool directoryNode::comparePointer(fileSystemNode* p1, fileSystemNode* p2){
    if(dynamic_cast<fileNode*> (p1) && dynamic_cast<directoryNode*> (p2)) {
        return false;
    }
    if(dynamic_cast<directoryNode*> (p1) && dynamic_cast<fileNode*> (p2)) {
        return true;
    }
        return *p1 <= *p2;
}
//PRE = riceve in input il puntatore corretto (anche nullo) a un nodo da aggiungere alla cartella
int directoryNode::addNode(fileSystemNode* f) {
    if(!f){//se il puntatore è nullo ritorna un'eccezione
        throw feedback(0, "Inserimento non riuscito, l'elemento da aggiungere non esiste");
    }
    if(f->contains(this)){
        throw feedback(0, "Inserimento non riuscito, impossibile inserire un nodo dentro sè stesso");
    }
    list<fileSystemNode*>::const_iterator it = subNodes.begin();
    int pos = 0;
    if(subNodes.size() > 0){
        for(it; it != subNodes.end() && comparePointer(*it, f); ++it){
            if((*it)->getName() == f->getName()){ //controllo che il nodo successivo non abbia lo stesso nome
                throw feedback(0, "Inserimento non riuscito, è presente un file o una cartella con lo stesso nome");
            }
            ++pos;
        }
        for(auto it2 = it; it2 != subNodes.end(); ++it2){
            if((*it2)->getName() == f->getName()){
                throw feedback(0, "Inserimento non riuscito, è presente un file o una cartella con lo stesso nome");
            }
        }
    }
    subNodes.insert(it, f);
    if(f->getParent() != this){//se il parent è uguale a questa directory vuol dire che addNode è stata chiamata da setParent, quindi evito di chiamare la stessa funzione
                               //altrimenti se il parent è diverso allora è necessario chiamare setParent sul nodo figlio
        if(f->getParent()){//eseguo a questo punto il controllo per permettere alla funzione setParent di spostare nodi da una cartella all'altra
            subNodes.erase(it);
            throw feedback(0, "Inserimento non riuscito, il nodo risulta contenuto in un'altra directory");
        }
        f->setParent(this);
    }
    return pos;
}
//POST = inserisce un nuovo nodo nella cartella e lo pone in posizione corretta,
//       se il puntatore è nullo o il suo nome è gia presente solleva un'eccezione
bool directoryNode::existName(const string& name) const {
    for(auto it = subNodes.begin(); it != subNodes.end(); ++it){
        if((*it)->getName() == name){
            return true;
        }
    }
    return false;
}
//PRE = riceve in input un puntatore a nodo corretto e non nullo da rimuovere
int directoryNode::removeNode(fileSystemNode* f) {
    bool found = false;
    if(!f){
        throw feedback(0, "Nodo non rimosso perchè inesistente");
    }
    if(!subNodes.size()){
        throw feedback(0, "Nodo non rimosso, la cartella è vuota");
    }
    int pos = -1;
    for(list<fileSystemNode*>::const_iterator it = subNodes.begin(); it != subNodes.end() && !found && comparePointer(*it, f); ++it){
        ++pos;
        if((*it)->getName() == f->getName()){
            found = true;
            subNodes.erase(it);
            if(f->getParent() == this){
                f->setParent(0);
            }
            return pos;
        }
    }
    return -1;
}
/*POST = rimuove il nodo ricevuto in input se esso è presente e se il puntatore non è nullo e ha settare correttamente il parent del nodo
         solo se non lo è già stato fatto, quindi se la funzione non è stata chiamata da setParent.
         solleva eccezione se il puntantore a nodo è nullo, se la cartella è vuota o se il nodo non si trova all'interno della cartella
 */
directoryNode* directoryNode::clone(fileSystemNode *parent) const{
    directoryNode *dir = new directoryNode(*this);
    dir->setParent(parent);
    return dir;
}
bool directoryNode::exist(fileSystemNode* f) const {
    for(list<fileSystemNode*>::const_iterator it = subNodes.begin(); it != subNodes.end() && comparePointer(*it, f); ++it){
        if(*it == f){
            return true;
        }
    }
    return false;
}
bool directoryNode::contains(fileSystemNode *f) const {
    if(this == f){
        return true;
    }
    if(exist(f)){
        return true;
    }
    for(auto it = subNodes.begin(); it != subNodes.end(); ++it){
        directoryNode* dir = dynamic_cast<directoryNode*>(*it);
        if(dir && dir->contains(f)){
            return true;
        }
    }
    return false;
}
string directoryNode::getType() const {
    return "directoryNode";
}
list<fileSystemNode*> directoryNode::openDirectory() const {
    return subNodes;
}
void directoryNode::copy(const list<fileSystemNode*>& l, fileSystemNode *parent){
    for (list<fileSystemNode*>::const_iterator it = l.begin(); it != l.end(); ++it) {
        (*it)->clone(parent);
    }
}
void directoryNode::drop(){
    list<fileSystemNode*>::const_iterator it = subNodes.begin();
    while(it != subNodes.end()){
        delete *it;
        it = subNodes.begin();
    }
}
directoryNode::~directoryNode(){
    drop();
}