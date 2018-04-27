//
// Created by michele on 08/03/17.
//

#ifndef CLOUD_2_0_STANDARDUSER_H
#define CLOUD_2_0_STANDARDUSER_H

#include "user.h"

class standardUser : public user{
private:
    static byteSize maxUpload;
    static byteSize maxUploadFile;
public:
    standardUser(const string&, const string&, const string&, const string&);
    virtual standardUser* clone() const;
    virtual string getUserInformation() const;
    virtual int upLoad(fileSystemNode*) const;
    virtual vector<fileSystemNode*> userSearch(const searchNode&) const;
    virtual string getType() const;
};


#endif //CLOUD_2_0_STANDARDUSER_H
