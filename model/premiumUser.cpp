//
// Created by michele on 09/03/17.
//

#include "premiumUser.h"
byteSize premiumUser::maxUpload = byteSize(15, "GB");
premiumUser::premiumUser(const string &n, const string &l, const string &e, const string &p) : standardUser(n, l, e, p) {}
premiumUser::premiumUser(standardUser* u) : standardUser(*u) {}
string premiumUser::getUserInformation() const{
    string s = "";
    if(!getRoot()){
        s = "\nL'utente non è ancora stato associato ad uno spazio di arciviazione";
    }
    return  "Tipo utente: premium\n" + user::getUserInformation() +
            "\nSpazio di archiviazione: " + maxUpload.getSize() + s;
}
premiumUser* premiumUser::clone() const {
    return new premiumUser(*this);
}
int premiumUser::upLoad(fileSystemNode *file) const{
    if(getCurrentSpace() + file->getSize() > maxUpload) {
        throw feedback(0, "Caricamento non riuscito, hai superato lo spazio di archiviazione massimo, che corrisponde a " +
                       maxUpload.getSize());
    }
    return getCurrentDirectory()->addNode(file);
}
vector<fileSystemNode*> premiumUser::userSearch(const searchNode& info) const {
    vector<fileSystemNode*> v;
    searchFunctor search(true);
    addSearchNode(&v, getCurrentDirectory(), search, info);
    return v;
}
string premiumUser::getType() const {
    return "premiumUser";
}