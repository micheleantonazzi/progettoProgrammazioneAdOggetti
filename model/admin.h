//
// Created by michele on 09/03/17.
//

#ifndef CLOUD_2_0_ADMIN_H
#define CLOUD_2_0_ADMIN_H


#include "user.h"
class admin : public user{
public:
    admin(const string&, const string&, const string&, const string&);
    virtual admin* clone() const;
    virtual string getUserInformation() const;
    virtual int upLoad(fileSystemNode*) const;
    virtual vector<fileSystemNode*> userSearch(const searchNode&) const;
    virtual string getType() const;
};

#endif //CLOUD_2_0_ADMIN_H
