//
// Created by michele on 31/03/17.
//

#ifndef CLOUD_2_0_MYLOGINWIDGET_H
#define CLOUD_2_0_MYLOGINWIDGET_H


#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include "labelClicked.h"
#include <string>
using std::string;
class myLoginWidget : public QWidget{
    Q_OBJECT
private:
    QLineEdit *lineEmail;
    QLineEdit *linePassword;
    QLineEdit *lineName;
    QLineEdit *linePassword2;
    QLabel *labelPassword2;
    QLabel *labelControl;
    QLabel *labelName;
    QLabel *labelLastName;
    QLineEdit *lineLastName;
    QPushButton *buttonSubmit;
    labelClicked *labelChange;
    QLabel *labelType;
    QComboBox *comboType;
    QFormLayout *layout;
public slots:
    void changeForm();
    void control();
public:
    myLoginWidget(QWidget* = 0);
    string getEmail() const;
    string getPassword() const;
    string getName() const;
    string getLastName() const;
    int getComboIndex() const;
    ~myLoginWidget();
};


#endif //CLOUD_2_0_MYLOGINWIDGET_H
