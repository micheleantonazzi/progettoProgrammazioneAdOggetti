//
// Created by michele on 09/03/17.
//

#ifndef CLOUD_2_0_PREMIUMUSER_H
#define CLOUD_2_0_PREMIUMUSER_H

#include "standardUser.h"
class premiumUser : public standardUser{
private:
    static byteSize maxUpload;
public:
    premiumUser(const string&, const string&, const string&, const string&);
    premiumUser(standardUser*);
    virtual premiumUser* clone() const;
    virtual string getUserInformation() const;
    virtual int upLoad(fileSystemNode*) const;
    virtual vector<fileSystemNode*> userSearch(const searchNode&) const;
    virtual string getType() const;
};


#endif //CLOUD_2_0_PREMIUMUSER_H
