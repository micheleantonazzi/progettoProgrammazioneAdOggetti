//
// Created by michele on 27/02/17.
//

#ifndef CLOUD_2_0_BYTESIZE_H
#define CLOUD_2_0_BYTESIZE_H

#include <iostream>
#include <string>

using std::string;
using std::ostream;

class byteSize {
    friend bool operator> (const byteSize&, const byteSize&);
    friend bool operator< (const byteSize&, const byteSize&);
    friend byteSize operator+ (const byteSize&, const byteSize&);
    friend ostream& operator<< (ostream&, const byteSize&);
    friend bool operator <= (const byteSize&, const byteSize&);
private:
    unsigned long long int byte;
    static string sigla[];
public:
    byteSize(unsigned long long int = 0, const string& = "");
    byteSize(const string&);
    byteSize& operator+= (const byteSize&);
    unsigned long long int getNByte() const; //solo per backup su xml
    string getSize() const;
    bool isNull() const;
    operator std::string() const;
};


#endif //CLOUD_2_0_BYTESIZE_H
