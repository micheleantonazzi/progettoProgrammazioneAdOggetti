//
// Created by michele on 27/02/17.
//

#include "feedback.h"
feedback::feedback(bool b, const string& m) : good(b), message(m) {}
feedback::feedback(const string& m) : message(m) {}
void feedback::setTrue() {
    good = 1;
}
void feedback::setFalse() {
    good = 0;
}
feedback::operator bool() const{
    return good;
}
bool feedback::operator! () const{
    return !good;
}
ostream& operator<< (ostream& os, const feedback& f){
    return os << f.message;
}
feedback::operator std::string() const{
    return message;
}
feedback& feedback::operator+= (const feedback& f){
    message = message + " " + f.message;
    return *this;
}