//
// Created by michele on 31/03/17.
//

#include <QtWidgets/QMessageBox>
#include "myLoginWidget.h"
#include "model/feedback.h"
myLoginWidget::myLoginWidget(QWidget *parent) : QWidget(parent), lineEmail(new QLineEdit()), linePassword(new QLineEdit()),
                                            linePassword2(new QLineEdit(this)), labelPassword2(new QLabel("Password*", this)),
                                            lineName(new QLineEdit(this)), lineLastName(new QLineEdit(this)),
                                            labelName(new QLabel("Nome", this)), labelChange(new labelClicked(this)),
                                            labelType(new QLabel("Tipo utente", this)), labelLastName(new QLabel("Cognome", this)),
                                            buttonSubmit(0),comboType(new QComboBox(this)), layout(new QFormLayout(this)),
                                            labelControl(new QLabel("", this)){
    comboType->addItem("Standard");
    comboType->addItem("Premium");
    layout->addRow(new QLabel("Email", this) , lineEmail);
    layout->addRow(new QLabel("Password", this), linePassword);
    linePassword->setEchoMode(QLineEdit::Password);
    linePassword2->setEchoMode(QLineEdit::Password);
    layout->addRow(labelPassword2, linePassword2);
    layout->addRow(labelName, lineName);
    layout->addRow(labelLastName, lineLastName);
    layout->addRow(labelType, comboType);
    layout->addRow(labelControl);
    layout->addRow(labelChange);
    labelChange->setStyleSheet("QLabel {color : blue; text-decoration: underline;}");
    labelControl->setStyleSheet("QLabel {color : red;}");
    labelChange->setText("Effettua login");
    setLayout(layout);
    connect(labelChange, SIGNAL(clicked()), this, SLOT(changeForm()));
    connect(lineEmail, SIGNAL(textEdited(const QString &)), this, SLOT(control()));
    connect(linePassword, SIGNAL(textEdited(const QString &)), this, SLOT(control()));
    connect(linePassword2, SIGNAL(textEdited(const QString &)), this, SLOT(control()));
    connect(lineName, SIGNAL(textEdited(const QString &)), this, SLOT(control()));
    connect(lineLastName, SIGNAL(textEdited(const QString &)), this, SLOT(control()));
    changeForm();
}
void myLoginWidget::changeForm() {
    delete buttonSubmit;
    buttonSubmit = new QPushButton(this);
    if(labelChange->text() == "Effettua login"){
        labelChange->setText("Registrati");
        buttonSubmit->setText("Login");
        labelPassword2->hide();
        layout->removeWidget(labelPassword2);
        linePassword2->hide();
        layout->removeWidget(linePassword2);
        labelName->hide();
        layout->removeWidget(labelName);
        lineName->hide();
        layout->removeWidget(lineName);
        labelLastName->hide();
        layout->removeWidget(labelLastName);
        lineLastName->hide();
        layout->removeWidget(lineLastName);
        labelType->hide();
        layout->removeWidget(labelType);
        comboType->hide();
        labelControl->hide();
        layout->removeWidget(labelControl);
        layout->removeWidget(comboType);
        layout->insertRow(3, buttonSubmit);
        connect(buttonSubmit, SIGNAL(clicked()), parentWidget(), SLOT(slotLogin()));
    }
    else if(labelChange->text() == "Registrati"){
        labelChange->setText("Effettua login");
        labelPassword2->show();
        linePassword2->show();
        labelName->show();
        labelLastName->show();
        lineName->show();
        lineLastName->show();
        labelType->show();
        comboType->show();
        buttonSubmit->setText("Registrati");
        layout->insertRow(2, labelPassword2, linePassword2);
        layout->insertRow(3, labelName, lineName);
        layout->insertRow(4, labelLastName, lineLastName);
        layout->insertRow(5, labelType, comboType);
        layout->insertRow(6, buttonSubmit);
        connect(buttonSubmit, SIGNAL(clicked()), parentWidget(), SLOT(slotAddUser()));
        control();
    }
    adjustSize();
    parentWidget()->adjustSize();
}
void myLoginWidget::control(){
    if(buttonSubmit->text() == QString::fromStdString("Registrati")){//esegue il controllo solo se sono nella sezione di registrazione
        feedback errors(1, "Attenzione");
        QRegExp regExpEmail("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", Qt::CaseInsensitive);
        if(!regExpEmail.exactMatch(lineEmail->text())){
            errors += feedback("\nEmail non corretta");
            errors.setFalse();
        }
        if(lineName->text().isEmpty()){
            errors += feedback("\nInserire il proprio nome");
            errors.setFalse();
        }
        if(lineLastName->text().isEmpty()){
            errors += feedback("\nInserire il proprio cognome");
            errors.setFalse();
        }
        if(linePassword->text().length() < 5){
            errors += feedback("\nPassword troppo corta");
            errors.setFalse();
        }
        if(linePassword->text() != linePassword2->text()){
            errors += feedback("\nLe password non corrispondono");
            errors.setFalse();
        }
        if(errors){
            layout->removeWidget(labelControl);
            labelControl->setVisible(false);
            buttonSubmit->setDisabled(false);
        }else{
            labelControl->setText(QString::fromStdString(errors));
            labelControl->setVisible(true);
            if(layout->indexOf(labelControl) < 0){
                layout->insertRow(6, labelControl);
            }
            buttonSubmit->setDisabled(true);
        }
        adjustSize();
        parentWidget()->adjustSize();
    }
}
string myLoginWidget::getEmail() const {
    return lineEmail->text().toStdString();
}
string myLoginWidget::getPassword() const {
    return linePassword->text().toStdString();
}
string myLoginWidget::getName() const {
    return lineName->text().toStdString();
}
string myLoginWidget::getLastName() const {
    return lineLastName->text().toStdString();
}
int myLoginWidget::getComboIndex() const {
    return comboType->currentIndex();
}
myLoginWidget::~myLoginWidget() {}
