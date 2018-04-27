//
// Created by michele on 09/03/17.
//

#include "admin.h"
admin::admin(const string &n, const string &l, const string &e, const string &p) : user(n, l, e, p){}
string admin::getUserInformation() const{
    string s;
    if(getRoot()){
        std::pair<int, int> num = getRoot()->countNodes();
        int users = getRoot()->openDirectory().size();
        s = "\nNumero di utenti: " + std::to_string(users)+
            "\nSottocartelle: " + std::to_string(num.first - users) +
            "\nFile: " + std::to_string(num.second);
    }
    else{
        s = "\nL'utente non è ancora associato ad uno spazio di archiviazione";
    }
    return  "Tipo utente: administrator\nEmail: " + getEmail() + "\nNome: " + getName() + "\nCognome: " + getLastName() + s;
}
int admin::upLoad(fileSystemNode *node) const{
    if(getCurrentDirectory() == getRoot()){
        throw feedback(0, "Caricamento non riuscito, non è possibile caricare file o cartelle nello spazio dedicato alle root degli utenti");
    }
    return getCurrentDirectory()->addNode(node);
}
vector<fileSystemNode*> admin::userSearch(const searchNode& info) const {
    vector<fileSystemNode*> v;
    searchFunctor search(true);
    addSearchNode(&v, getCurrentDirectory(), search, info);
    return v;
}
admin* admin::clone() const {
    return new admin(*this);
}
string admin::getType() const {
    return "admin";
}