//
// Created by michele on 16/03/17.
//

#include <QtWidgets/QMessageBox>
#include "cloudModel.h"
#include "directoryNodeShared.h"
#include "fileNode.h"
#include "fileNodeShared.h"
#include "premiumUser.h"

admin* cloudModel::defaultAdmin = new admin("Francesco", "Ranzato", "ranzato@gmail.com", "12345");
rootNode* cloudModel::defaultRoot = new rootNode(defaultAdmin->getEmail(), defaultAdmin->getUserName());
cloudModel::cloudModel(rootNode *r, const list<user*>& u) : root(r), users(cloneUsers(u)), userLogged(0){}
list<user*> cloudModel::cloneUsers(list<user*> l) const {
    list<user*> ret;
    for(auto it = l.begin(); it != l.end(); ++it){
        ret.push_back((*it)->clone());
    }
    return ret;
}
void cloudModel::logout() {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    userLogged->logout();
    userLogged = 0;
}
string cloudModel::getUserName() const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return userLogged->getUserName();
}
list<fileSystemNode*> cloudModel::openDir() const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return userLogged->getCurrentDirectory()->openDirectory();
}
int cloudModel::addNode(fileSystemNode* f) {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return userLogged->upLoad(f);
}
void cloudModel::deleteNode(fileSystemNode* f) const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    userLogged->deleteNode(f);
}
bool cloudModel::existUser(const string &email) const {
    return root->existName(email);
}
string cloudModel::getUserLoggedEmail() const {
    return userLogged->getEmail();
}
void cloudModel::share(const string& email, fileSystemNode* node, const string& newName) const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    if (!node) {
        throw feedback(0, "Attenzione, impossibile condividere un elemento inesistente");
    }
    bool found = false;
    for (auto it = ++users.begin(); it != users.end() && (*it)->getEmail() <= email; ++it) {
        if ((*it)->getEmail() == email) {
            directoryNode *dir = dynamic_cast<directoryNode *>(node);
            fileNode *file = dynamic_cast<fileNode *>(node);
            fileSystemNode *nodeShared = 0;
            try {
                if (dir) {
                    nodeShared = new directoryNodeShared(dir->clone(), userLogged->getUserName());
                } else if (file) {
                    nodeShared = new fileNodeShared(file->clone(), userLogged->getUserName());
                }
                if (newName != "") {
                    nodeShared->rename(newName);
                }
                (*it)->getRoot()->addNode(nodeShared);
                found = true;
            }
            catch (feedback) {
                delete nodeShared;
                throw feedback(0, "Attenzione, l'elemento non è stato condiviso perchè"
                        " il destinatario possiede già un  elemento con lo stesso nome");
            }
        }
    }
    if (!found) {
        throw feedback(0, "Attenzione, utente non trovato");
    }
}
directoryNode* cloudModel::navigateTree(fileSystemNode* f) const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    directoryNode* directory = dynamic_cast<directoryNode*>(f);
    if(!directory){
        throw feedback(0, "Attenzione, impossibile navigare aprendo un file");
    }
    return userLogged->navigate(directory);
}
directoryNode* cloudModel::getCurrentDirectory() const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return userLogged->getCurrentDirectory();
}
directoryNode* cloudModel::getParent() const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return userLogged->getParent();
}
string cloudModel::getUserLoggedName() const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return userLogged->getName();
}
string cloudModel::getUserLoggedLastName() const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return userLogged->getLastName();
}
string cloudModel::getAccountInformation() const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return userLogged->getUserInformation();
}
bool cloudModel::isTheTop() const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return userLogged->isTheTop();
}
bool cloudModel::isUserDirectory() const{
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return adminIsLogged() && userLogged->getCurrentDirectory() == userLogged->getRoot();
}
bool cloudModel::adminIsLogged() const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return typeid(*userLogged) == typeid(admin);
}
void cloudModel::setDefault(){
    destroy();
    users.push_back(defaultAdmin->clone());
    root = defaultRoot->clone();
    (*users.begin())->setRoot(root);
}
void cloudModel::upgradeAccount(){
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    if(typeid(*userLogged) != typeid(standardUser)){
        throw feedback(0, "Attenzione, l'utente è gia premium");
    }
    user* newUser = new premiumUser(static_cast<standardUser*>(userLogged));
    auto it = ++users.begin();
    for(it; it != users.end() && *it != userLogged; ++it){}
    it = users.erase(it);
    users.insert(it, newUser);
    delete userLogged;
    userLogged = newUser;
};
vector<fileSystemNode*> cloudModel::search(searchNode& searchInformation) const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return userLogged->userSearch(searchInformation);
}
fileSystemNode* cloudModel::getTree() const {
    return root; //non effettuo la copia profonda per evitare di specare memoria
}
void cloudModel::setRoot(fileSystemNode* r){
    rootNode* radice = dynamic_cast<rootNode*>(r);
    if(!radice){
        throw feedback(0, "Attenzione, il nodo non è una root");
    }
    root = radice;
}
void cloudModel::destroy() {
    delete root;
    for (list<user*>::const_iterator it = users.begin(); it != users.end();) {
        delete *it;
        it = users.erase(it);
    }
}
list<user*> cloudModel::getUsers() const {
    return cloneUsers(users);
}
void cloudModel::login(const string& email, const string& password) {
    bool logged = false;
    list<user*>::const_iterator it = users.begin();
    for(it; it != users.end() && !logged; ++it) {
        if((*it)->login(email, password)){
            userLogged = *it;
            logged = true;
        }
    }
    if(!logged){
        throw feedback(0, "Accesso non riuscito, email e password errate");
    }
}
//PRE = riceve in input un puntatore a utente, anche nullo
void cloudModel::addUser(user *u) {
    if(!u){
        throw feedback(0, "Attenzione, inserire un utente valido");
    }
    list<user*>::const_iterator it = ++users.begin();//aggiungo utenti dopo l'admin
    for(it; it != users.end() && (*it)->getEmail() <= u->getEmail(); ++it) {
        if((*it)->getEmail() == u->getEmail()){
            throw feedback(0, "Registrazione non riuscita, l'email è gia utilizzata");
        }
    }
    users.insert(it, u->clone());
}
/*POST = -se il puntatore è nullo solleva eccezione
         -se l'email del nuovo utente è gia utilizzata solleva eccezione
         -se non viene sollevata nssuna eccezione il ciclo for si ferma esattamente nella posizione corretta nella quale inserire l'utente
          in ordine alfabetico secondo le email e inserisce una nuova root al secondo livello dell'albero, che verrà automaticamente ordinata
          dal metodo addNode()
*/
bool cloudModel::completeSearch() const {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    return !(typeid(*userLogged) == typeid(standardUser));
}
void cloudModel::changeAccountInformation(const string& email, const string& name, const string& lastName, const string& password, const string& password1, const string& password2) {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    if(!userLogged->controlPassword(password)){
        throw feedback(0, "Attenzione, per verificare la tua identità inserisci la password corretta");
    }
    feedback errors(1, "Attenzione");
    std::regex regEmail("^[a-zA-Z0-9][a-zA-Z0-9_.]+@[a-zA-Z0-9_]+.[a-zA-Z0-9_.]+$");
    if(!std::regex_match(email, regEmail)){
        errors.setFalse();
        errors += feedback("\nIndirizzo email non valido");
    }
    if(name.empty()){
        errors += feedback("\nInserire il proprio nome");
        errors.setFalse();
    }
    if(lastName.empty()){
        errors += feedback("\nInserire il proprio cognome");
        errors.setFalse();
    }
    if(!password1.empty() || !password2.empty()){
        if(password1.length() < 5){
            errors += feedback("\nPassword troppo corta");
            errors.setFalse();
        }
        if(password1 != password2){
            errors += feedback("\nLe password non corrispondono");
            errors.setFalse();
        }
    }
    if(!errors){
        throw errors;
    }
    int pos = userLogged->changeEmail(email, password) + 1;
    ++pos;
    if(pos >= 0){
        users.remove(userLogged);
        auto it = ++users.begin();
        for(pos; pos > 0 && it != users.end(); --pos){
            ++it;
        }
        users.insert(it, userLogged);
    }
    userLogged->changeName(name, password);
    userLogged->changeLastName(lastName, password);
    if (!password1.empty()){
        userLogged->changePassword(password1, password2, password);
    }
}
int cloudModel::deleteUser(fileSystemNode* node) {
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    if(!adminIsLogged()){
        throw feedback(0, "Attenzione, per poter eliminare un utente devi essere amministratore");
    }
    rootNode* root = dynamic_cast<rootNode*>(node);
    if(!root){
        throw feedback(0, "Attenzione, per poter eliminare un utente bisogna selezionare la sua directory principale");
    }
    auto it = ++users.begin();
    int pos = 0;
    for(it; it != users.end() && (*it)->getEmail() <= root->getName(); ++it){
        if((*it)->getEmail() == root->getName()){
            delete (*it)->getRoot();
            delete *it;
            users.erase(it);
            return pos;
        }
        ++pos;
    }
    return -1;
}
void cloudModel::deleteUser(){
    if(!userLogged){
        throw feedback(0, "Attenzione, nessun utente loggato");
    }
    if(adminIsLogged()){
        throw feedback(0, "Attenzione, l'amministratore non può essere eliminato");
    }
    users.remove(userLogged);
    delete userLogged->getRoot();
    delete userLogged;
    logout();
}
void cloudModel::createUserRoot(user* u) const{
    rootNode* r = new rootNode(u->getEmail(), u->getUserName());
    try {
        root->addNode(r);
    }catch(feedback){
        throw feedback("Attenzione, l'utente è già in possesso di una root");
    }
    for (auto it = users.begin(); it != users.end(); ++it) {
        std::cout << (*it)->getEmail() << std::endl;
    }
    bool found = false;
    for(auto it = ++users.begin(); it != users.end() && !found; ++it){//assegno la root all'utente corrispondente, partendo dalla poszione 1 della lista, saltando coiè l'admin
        if((*it)->getEmail() == r->getName()){
            (*it)->setRoot(r);
            found = true;
        }
    }

}
void cloudModel::setAdmin(admin* a) {
    if(!a){
        throw feedback(0, "Attenzione, impossibile inserire un admin vuoto");
    }
    users.insert(users.begin(), a->clone());
}
//la funzione setUserRoot abbina ogni utente alla sua root, effettuando però dei controlli necessari a mantenere l'integrità della struttura dati
// che può essere sporcata per esempio dalla lettura di un xml errato
void cloudModel::setUserRoot() const{
    admin *ad = dynamic_cast<admin*>(*users.begin());
    if(!ad){//controllo che l'admin sia sempre presente
        feedback(0, "Errore, admin non presente");
    }
    list<fileSystemNode*> roots = root->openDirectory();
    if(roots.size() != users.size() - 1){//se il numero degli utenti e delle root non corrisponde sollevo eccezione
        throw feedback(0, "Attenzione, il numero di utenti e il numero delle root non corrisponde");
    }
    ad->setRoot(root);//setto la root dell'admin, che è sempre il primo della lista di utenti ed è unico
    auto itUsers = ++users.begin();
    auto itRoots = roots.begin();
    while(itUsers != users.end()) {
        (*itUsers)->setRoot(*itRoots);//può sollevare eccezioni
        ++itRoots;
        ++itUsers;
    }
}
cloudModel::~cloudModel(){
    destroy();
}
