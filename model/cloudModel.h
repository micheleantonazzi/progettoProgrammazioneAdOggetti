//
// Created by michele on 16/03/17.
//

#ifndef CLOUD_2_0_CLOUDMODEL_H
#define CLOUD_2_0_CLOUDMODEL_H

#include "rootNode.h"
#include "admin.h"
class cloudModel {
private:
    static rootNode* defaultRoot;
    static admin* defaultAdmin;
    user* userLogged;
    rootNode* root;//root dell'intero albero che compone il cloud
    list<user*> users;//lista di utenti che possono accedere al cloud
    list<user*> cloneUsers(list<user*>) const;
    void destroy();
public:
    cloudModel(rootNode* = 0, const list<user*>& = list<user*>());
    cloudModel(const cloudModel&) = delete;//il modello di un cloud non può essere clonato, copiato o assegnato
    cloudModel& operator= (const cloudModel&) = delete;
    list<user*> getUsers() const;
    void setAdmin(admin*);//usato per settare l'amministratore, lo uso principalmente nelle operazioni di lettura da xml
    void setDefault();
    void createUserRoot(user*) const;
    int addNode(fileSystemNode*);
    bool completeSearch() const;
    void deleteNode(fileSystemNode*) const;
    directoryNode* navigateTree(fileSystemNode*) const;
    directoryNode* getParent() const;
    bool isTheTop() const;
    bool existUser(const string &) const;
    string getUserLoggedEmail() const;
    void share(const string&, fileSystemNode*, const string& = "") const;
    directoryNode* getCurrentDirectory() const;
    bool adminIsLogged() const;
    bool isUserDirectory() const;
    int deleteUser(fileSystemNode*);
    string getAccountInformation() const;
    void deleteUser();
    vector<fileSystemNode*> search(searchNode&) const;
    string getUserName() const;
    void logout();
    string getUserLoggedName() const;
    string getUserLoggedLastName() const;
    void upgradeAccount();
    void changeAccountInformation(const string&, const string&, const string&, const string&, const string&, const string&);
    list<fileSystemNode*> openDir() const;//ritorna una lista contenente i sottonodi della cartella corrente dell'utente loggato
    fileSystemNode* getTree() const; //questo metodo è usato solamente per facilitare il backup su xml
    void setRoot(fileSystemNode*);  //questo metodo è usato solamente per facilitare il restore da xml,
                                   //solleva eccezione se la directory non è una root
    void login(const string&, const string&);//solleva eccezione se la password è errata o se l'utente non esiste
    void addUser(user*);//inserisce un nuovo utente nella posizione corretta, cioè in ordine alfabetico, rispettando così l'ordine delle cartelle di root
                        //solleva eccezione se un l'utente da inserire è nullo oppure la sua email è gia stata utilizzata
    void setUserRoot() const;//Questa funzione permette di asseganre a ogni utente il puntatore alla sua root, inoltre se trova problemi imposta il modello a default
    ~cloudModel();
};


#endif //CLOUD_2_0_CLOUDMODEL_H
