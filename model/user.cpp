//
// Created by michele on 08/03/17.
//

#include "user.h"
#include "fileNode.h"
#include "admin.h"

user::user(const string &n, const string &l, const string &e, const string &p) :
        name(n), lastName(l), email(e), password(p), currentSpace(0), root(0){}
string user::getUserName() const{
    return name + " " + lastName;
}
string user::getEmail() const{
    return email;
}
string user::getName() const {
    return name;
}
string user::getLastName() const {
    return lastName;
}
string user::getPassword() const {
    return password;
}
byteSize user::getCurrentSpace() const{
    return currentSpace;
}
directoryNode* user::getCurrentDirectory() const{
    return currentDirectory;
}
rootNode* user::getRoot() const{
    return root;
}
void user::logout() {
    currentDirectory = root;
}
int user::changeEmail(const string& newEmail, const string& pass) {
    if(password != pass){
        throw feedback(0, "Impossibile cambiare l'email, password errata");
    }
    if(email != newEmail){
        std::regex regEmail("^[a-zA-Z0-9][a-zA-Z0-9_.]+@[a-zA-Z0-9_]+.[a-zA-Z0-9_.]+$");
        if(!std::regex_match(newEmail, regEmail)){
            throw feedback(0, "Attenzione, l'email non è corretta");
        }
        if(dynamic_cast<admin*>(this)){
            if (root->existName(newEmail)){
                throw feedback(0, "Attenzione, l'email risulta giu in uso");
            }
        }
        else if(newEmail == root->getParent()->getName() || (dynamic_cast<directoryNode*>(root->getParent()))->existName(newEmail)){
            throw feedback(0, "Attenzione, l'email risulta giu in uso");
        }
        email = newEmail;
        return root->rename(email);
    }
    return -1;
}
void user::changeLastName(const string& lName, const string& pass) {
    if(password != pass){
        throw feedback(0, "Impossibile cambiare il cognome, pasword errata");
    }
    if(lastName != lName){
        if(lName.empty()){
            throw feedback(0, "Attenzione, il cognome non può essere vuoto");
        }
        lastName = lName;
        root->changeUserName(getUserName());
    }
}
void user::changeName(const string& n, const string& pass) {
    if(password != pass){
        throw feedback(0, "Impossibile cambiare il cognome, pasword errata");
    }
    if(name != n){
        if(n.empty()){
            throw feedback(0, "Attenzione, il nome non può essere vuoto");
        }
        name = n;
        root->changeUserName(getUserName());
    }
}
void user::changePassword(const string& password1, const string& password2, const string& pass) {
    if(password != pass){
        throw feedback(0, "Impossibile cambiare la password perchè errata");
    }
    if(!(password1 == password2 && password1 == pass && password2 == pass)){
        if(password1.length() < 5){
            throw feedback(0, "Impossibile cambiare la password perchè troppo corta");
        }
        if(password1 != password2){
            throw feedback(0, "Impossibile cambiare la password perchè i due valori inseriti non corrispondono");
        }
        password = password1;
    }
}
directoryNode* user::navigate(directoryNode* dir) {
    if(root != dir && !root->contains(dir)){
        throw feedback(0, "Attenzione, la cartella non appartiene a questo utente");
    }
    currentDirectory = dir;
    return dir;
}
void user::deleteNode(fileSystemNode* f) const {
    currentDirectory->removeNode(f);
}
directoryNode* user::getParent() {
    if(currentDirectory == root){
        throw feedback(0, "Attenzione, ti trovi già al livello più alto");
    }
    currentDirectory = dynamic_cast<directoryNode*>(currentDirectory->getParent());
    return currentDirectory;
}
bool user::isTheTop() const {
    return currentDirectory == root;
}
string user::getUserInformation() const{
    string s = "";
    if(getRoot()){
        std::pair<int, int> num = getRoot()->countNodes();
        s = "\nSottocartelle: " + std::to_string(--num.first) +
            "\nFile: " + std::to_string(num.second) +
            "\nSpazio occupato: " + getRoot()->getSize().getSize();
    }
    return "Email: " + email + "\nNome: " + name + "\nCognome: " + lastName + s;
}
bool user::operator== (const user &u) const {
    return email == u.email && name == u.name && u.lastName == lastName && password == u.password;
}
bool user::login(const string& e, const string& p) {
    return email == e && password == p;
}
void user::setRoot(fileSystemNode *node) {
    rootNode *r = dynamic_cast<rootNode*>(node);
    if(!r){
        throw feedback(0, "Attenzione, il nodo non è una root");
    }
    if(email != r->getName() || getUserName() != r->getUserName()){
        throw feedback(0, "Attenzione, utente e root non corrispondono");
    }
    root = r;
    currentDirectory = root;
}
bool user::controlPassword(const string& p) const {
    return password == p;
}
void user::addSearchNode(vector<fileSystemNode*>* v, fileSystemNode* n, const searchFunctor& search, const searchNode& info) {
    directoryNode* dir = dynamic_cast<directoryNode*>(n);
    if(dir){
        list<fileSystemNode*> subNodes = dir->openDirectory();
        for(auto it = subNodes.begin(); it != subNodes.end(); ++it){
            if (search(*it, info)){
                v->push_back(*it);
            }
            if(dynamic_cast<directoryNode*>((*it))) {
                addSearchNode(v, *it, search, info);
            }
        }
    }
}
user::~user() {}

user::searchFunctor::searchFunctor(bool b) : completeSearch(b) {}
bool user::searchFunctor::operator()(fileSystemNode* node, const searchNode& info) const {
    if((dynamic_cast<directoryNode*> (node) && info.getDirectory()) || (dynamic_cast<fileNode*> (node) && info.getFile())) {
        if (completeSearch) {
            if ((info.getMinSize().isNull() && info.getMaxSize().isNull()) ||
                (info.getMinSize() <= node->getSize() && node->getSize() <= info.getMaxSize())) {
                if (info.isEmpty() || std::regex_match(node->getName(), info.getReg())) {
                    return true;
                }
            }
        }
        else if (info.isEmpty() || std::regex_match(node->getName(), info.getReg())) {
            return true;
        }
    }
    return false;
}
