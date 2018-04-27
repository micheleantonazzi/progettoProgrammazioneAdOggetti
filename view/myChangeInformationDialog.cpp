//
// Created by michele on 20/05/17.
//

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "myChangeInformationDialog.h"
myChangeInformationDialog::myChangeInformationDialog(QWidget* p, const string& email, const string& name, const string& lastName, bool premium)
                                                    : QDialog(p), lineEmail(new QLineEdit(this)), lineName(new QLineEdit(this)),
                                                      lineLastName(new QLineEdit(this)), linePassword(new QLineEdit(this)),
                                                      lineNewPassword1(new QLineEdit(this)), lineNewPassword2(new QLineEdit(this)), isPremium(premium) {
    setWindowTitle("Modifica i dati del tuo account");
    setMinimumWidth(parentWidget()->width() / 3);
    QFormLayout *form = new QFormLayout(this);
    lineEmail->setText(QString::fromStdString(email));
    form->addRow("Email", lineEmail);
    lineName->setText(QString::fromStdString(name));
    form->addRow("Nome", lineName);
    lineLastName->setText(QString::fromStdString(lastName));
    form->addRow("Cognome", lineLastName);
    form->addRow("Password", linePassword);
    linePassword->setEchoMode(QLineEdit::Password);
    lineNewPassword1->setEchoMode(QLineEdit::Password);
    lineNewPassword2->setEchoMode(QLineEdit::Password);
    form->addRow("Nuova password", lineNewPassword1);
    form->addRow("Ripeti la nuova password", lineNewPassword2);
    QWidget *spacer = new QWidget(this);
    spacer->setFixedHeight(lineEmail->height() / 2);
    form->addRow(spacer);
    QWidget *widgetButtons = new QWidget(this);
    QHBoxLayout *layoutButtons = new QHBoxLayout(widgetButtons);
    layoutButtons->setMargin(0);
    QPushButton *buttonDeleteAccount = new QPushButton("Elimina account", widgetButtons);

    QPushButton *buttonConfirmSettings = new QPushButton("Conferma modifiche", widgetButtons);
    layoutButtons->addWidget(buttonDeleteAccount);
    if (!isPremium) {
        QPushButton *buttonUpgrade = new QPushButton("Passa a premium", widgetButtons);
        layoutButtons->addWidget(buttonUpgrade);
        connect(buttonUpgrade, SIGNAL(clicked()), parentWidget(), SLOT(slotUpgradeAccount()));
    }
    layoutButtons->addWidget(buttonConfirmSettings);
    connect(buttonConfirmSettings, SIGNAL(clicked()), parentWidget(), SLOT(slotConfirmSettings()));
    connect(buttonDeleteAccount, SIGNAL(clicked()), parentWidget(), SLOT(slotDeleteAccount()));
    form->addRow(widgetButtons);
}
string myChangeInformationDialog::getNewEmail() const {
    return lineEmail->text().toStdString();
}
string myChangeInformationDialog::getNewName() const {
    return lineName->text().toStdString();
}
string myChangeInformationDialog::getNewLastName() const {
    return lineLastName->text().toStdString();
}
string myChangeInformationDialog::getPassword() const {
    return linePassword->text().toStdString();
}
string myChangeInformationDialog::getNewPassword1() const {
    return lineNewPassword1->text().toStdString();
}
string myChangeInformationDialog::getNewPassword2() const {
    return lineNewPassword2->text().toStdString();
}