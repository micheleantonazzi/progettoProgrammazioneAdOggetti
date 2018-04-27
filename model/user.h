//
// Created by michele on 08/03/17.
//

#ifndef CLOUD_2_0_USER_H
#define CLOUD_2_0_USER_H

#include <string>
#include <vector>
#include "byteSize.h"
#include "rootNode.h"
#include "searchNode.h"
using std::string;
using std::vector;
class user {
private:
    string email;
    string password;
    string name;
    string lastName;
    byteSize currentSpace;
    rootNode* root;
    directoryNode* currentDirectory;
protected:
    class searchFunctor{
    private:
        bool completeSearch;
    public:
        searchFunctor(bool);
        bool operator() (fileSystemNode*, const searchNode&) const;
    };
    static void addSearchNode(vector<fileSystemNode*>*, fileSystemNode*, const searchFunctor&, const searchNode&);
public:
    user(const string&, const string&, const string&, const string&);
    string getUserName() const;
    string getName() const;
    string getLastName() const;
    string getPassword() const; //solo per fare il backup su xml
    string getEmail() const;
    byteSize getCurrentSpace() const;
    rootNode* getRoot() const;
    directoryNode* getCurrentDirectory() const;
    bool login(const string&, const string&);
    void setRoot(fileSystemNode*);
    directoryNode* navigate(directoryNode*);
    directoryNode* getParent();
    void logout();
    bool isTheTop() const;
    bool controlPassword(const string&) const;
    void deleteNode(fileSystemNode*) const;
    int changeEmail(const string&, const string&);
    void changePassword(const string&, const string&, const string&);
    void changeName(const string&, const string&);
    void changeLastName(const string&, const string&);
    virtual string getUserInformation() const;
    virtual user* clone() const = 0;
    virtual int upLoad(fileSystemNode*) const = 0;
    virtual string getType() const = 0;
    virtual vector<fileSystemNode*> userSearch(const searchNode&) const = 0;
    bool operator== (const user&) const;
    virtual ~user();
};


#endif //CLOUD_2_0_USER_H
