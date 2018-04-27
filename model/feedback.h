//
// Created by michele on 27/02/17.
//

#ifndef CLOUD_2_0_FEEDBACK_H
#define CLOUD_2_0_FEEDBACK_H

#include <iostream>
#include <string>
using std::string;
using std::ostream;

class feedback {
    friend ostream& operator<< (ostream&, const feedback&);
private:
    bool good;
    string message;
public:
    feedback(bool, const string&);
    feedback(const string&);
    void setFalse();
    void setTrue();
    feedback& operator+= (const feedback&);
    operator bool() const;
    bool operator! () const;
    operator std::string() const;
};


#endif //CLOUD_2_0_FEEDBACK_H
