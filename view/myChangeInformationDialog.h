//
// Created by michele on 20/05/17.
//

#ifndef CLOUD_2_0_MYCHANGEINFORMATIONDIALOG_H
#define CLOUD_2_0_MYCHANGEINFORMATIONDIALOG_H


#include <QtWidgets/QDialog>
#include <string>
using std::string;
class myChangeInformationDialog : public QDialog{
private:
    bool isPremium;
    QLineEdit* lineEmail;
    QLineEdit* lineName;
    QLineEdit* lineLastName;
    QLineEdit* linePassword;
    QLineEdit* lineNewPassword1;
    QLineEdit* lineNewPassword2;
public:
    myChangeInformationDialog(QWidget*, const string&, const string&, const string&, bool);
    string getNewEmail() const;
    string getNewName() const;
    string getNewLastName() const;
    string getPassword() const;
    string getNewPassword1() const;
    string getNewPassword2() const;
};


#endif //CLOUD_2_0_MYCHANGEINFORMATIONDIALOG_H
