//
// Created by michele on 27/02/17.
//

#include "byteSize.h"
#include "feedback.h"
#include <regex>
#include <cmath>
using std::regex;
string byteSize::sigla[] = {"byte", "KB", "MB", "GB", "TB"};
byteSize::byteSize(unsigned long long int nByte, const string& s) {
    if(s == ""){
        byte = nByte;
    }
    else{
        unsigned int i = 0, moltiplicatore = 1;
        for (; i < sigla->length() && s != sigla[i]; ++i) {
            moltiplicatore *= 1024;
        }
        if(i >= sigla->length()){
            byte = nByte;
        }
        else{
            byte = nByte * moltiplicatore;
        }
    }
}
bool byteSize::isNull() const {
    return byte ? false : true;
}
byteSize::byteSize(const string& s) : byte(0){
    regex control("[[:digit:]]+((.[[:digit:]]+ (KB|MB|GB|TB))| (byte|KB|MB|GB|TB))");
    if(std::regex_match(s, control)){
        unsigned int i = 0;
        string siglaStringa = s.substr(s.find(' ') + 1, s.length() - s.find(' '));
        for (; i < sigla->length() && siglaStringa != sigla[i]; ++i) {}
        byte = pow(1024, i) * std::stod(s);
    }
    else{
        byte = 0;
        throw feedback(0, "Attenzione, impossibile determinare una dimensione corretta");
    }
}
//questa funzione permette di minimizzare l'errore dovuto alle perazioni di divisione, la conversione in base diversa
string byteSize::getSize() const{
    unsigned long long int nByte = byte;
    unsigned long long int i = 0;
    for (; nByte > 1025; ++i) {
        nByte /= 1024;
    }
    string val = std::to_string(byte / pow(1024, i));
    val.substr(val.find(',') + 1, 3) == "000" ? val = val.substr(0, val.find(',')) : val = val.substr(0, val.find(',') + 4);
    return  val + " " + sigla[i];
}
unsigned long long int byteSize::getNByte() const {
    return byte;
}
byteSize::operator std::string() const{
    return getSize();
}
byteSize operator+ (const byteSize& b1, const byteSize& b2){
    byteSize ret;
    ret.byte = b1.byte + b2.byte;
    return ret;
}
ostream& operator<< (ostream& os, const byteSize& b){
    return os << b.getSize();
}
bool operator> (const byteSize& b1, const byteSize& b2){
    return b1.byte > b2.byte;
}
bool operator< (const byteSize& b1, const byteSize& b2){
    return b1.byte < b2.byte;
}
bool operator<= (const byteSize& b1, const byteSize& b2){
    return b1.byte <= b2.byte;
}
byteSize& byteSize::operator+= (const byteSize& b){
    byte += b.byte;
    return *this;
}
