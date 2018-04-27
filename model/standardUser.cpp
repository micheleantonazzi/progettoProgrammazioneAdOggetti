//
// Created by michele on 08/03/17.
//

#include "standardUser.h"
byteSize standardUser::maxUpload = byteSize(5, "GB");
byteSize standardUser::maxUploadFile = byteSize(300, "MB");
standardUser::standardUser(const string &n, const string &l, const string &e, const string &p) : user(n, l, e, p) {}
string standardUser::getUserInformation() const{
    string s = "";
    if(!getRoot()){
        s = "\nL'utente non è ancora stato associato ad uno spazio di arciviazione";
    }
    return "Tipo utente: standard\n" + user::getUserInformation() +
           "\nSpazio di archiviazione: " + maxUpload.getSize() +
           "\nDimensione massima file: " + maxUploadFile.getSize() + s;
}
int standardUser::upLoad(fileSystemNode *file) const{
    if(file->getSize() > maxUploadFile){
        throw feedback(0, "Caricamento non riuscito, il file non deve superare la dimensione di " + maxUploadFile.getSize());
    }
    if((getCurrentSpace() + file->getSize()) > maxUpload){
        throw feedback(0, "Caricamento non riuscito, il tuo spazio di archiviazione non può superare i " + maxUpload.getSize());
    }
    return getCurrentDirectory()->addNode(file);
}
vector<fileSystemNode*> standardUser::userSearch(const searchNode& info) const {
    vector<fileSystemNode*> v;
    searchFunctor search(false);
    addSearchNode(&v, getCurrentDirectory(), search, info);
    return v;
}
standardUser* standardUser::clone() const {
    return new standardUser(*this);
}
string standardUser::getType() const {
    return "standardUser";
}