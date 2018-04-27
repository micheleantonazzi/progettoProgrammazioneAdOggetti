//
// Created by michele on 24/03/17.
//

#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QXmlStreamReader>
#include <QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QtWidgets>
#include "cloudView.h"
#include "model/fileNodeShared.h"
#include "model/directoryNodeShared.h"
#include "model/premiumUser.h"
#include "myQTableWidgetItem.h"

cloudView::cloudView(cloudModel *m) : QMainWindow(0), model(m), moveNode(0), widgetLogin(0),
                                      userWidget(0), toolBar(0), searchDialog(0), searchTable(0),
                                      actionAddDir(0), actionAddFile(0), actionLevelUp(0), actionDelete(0), actionCopy(0),
                                      actionPaste(0), actionShare(0), actionRename(0), changeInformationDialog(0){
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(slotBackup()));
    try {
        restore();
    }catch(feedback feed){
        model->setDefault();
        QMessageBox::critical(this, "Errore", QString::fromStdString(feed) + ".\nSaranno impostati i valori di default");
    }
    setWidgetLogin();
}
void cloudView::setWidgetLogin(){
    setWindowState(Qt::WindowNoState);
    delete toolBar;
    delete userWidget;
    widgetLogin = new myLoginWidget(this);
    setCentralWidget(widgetLogin);
    widgetLogin->show();
    widgetLogin->setMinimumWidth(QApplication::desktop()->screenGeometry().width() / 7 * 2);
    adjustSize();
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
    setWindowTitle("Effettua il login");
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
}
void cloudView::setWidgetUser() {
    delete widgetLogin;
    userWidget = new myUserWidget(this, model->completeSearch());
    userWidget->show();
    createToolBar();
    disableActions();
    if(model->adminIsLogged()){
        disableAdminActions();
    }
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
    setCentralWidget(userWidget);
    model->isUserDirectory() ? userWidget->createUserTreeWidget(model->getCurrentDirectory()) : userWidget->createTreeWidget(model->getCurrentDirectory());
    openDirectory(model->getCurrentDirectory());
    setWindowState(Qt::WindowMaximized);
    setWindowTitle(QString::fromStdString("Benvenuto " + model->getUserName()));
}
void cloudView::slotLogin() {
    try{
        model->login(widgetLogin->getEmail(), widgetLogin->getPassword());
        setWidgetUser();
    }catch(feedback feed){
        QMessageBox::critical(this, "Attenzione", QString::fromStdString(feed));
    }
}
void cloudView::createToolBar() {
    toolBar = new QToolBar(this);
    actionAddDir = new QAction("Aggiungi cartella", toolBar);
    actionAddFile = new QAction("Aggiungi file", toolBar);
    actionLevelUp = new QAction("Livello superiore", toolBar);
    actionDelete = new QAction("Elimina", toolBar);
    actionCopy = new QAction("Copia", toolBar);
    actionPaste = new QAction("Incolla", toolBar);
    actionShare = new QAction("Condividi", toolBar);
    actionRename = new QAction("Rinomina", toolBar);
    QAction* actionSettings = new QAction("Impostazioni account", toolBar);
    QAction* actionAccountInformation = new QAction("Informazioni account", toolBar);
    QAction* actionLogout = new QAction("Logout", this);
    toolBar->setMovable(false);
    toolBar->show();
    const QIcon iconPadre = QIcon("./images/Su-50.png");
    actionLevelUp->setIcon(iconPadre);
    connect(actionLevelUp, SIGNAL(triggered()), this, SLOT(slotLevelUp()));
    actionLevelUp->setDisabled(true);
    toolBar->addAction(actionLevelUp);
    const QIcon iconAddFile = QIcon("./images/Nuovo_File-50.png");
    actionAddFile->setIcon(iconAddFile);
    toolBar->addAction(actionAddFile);
    connect(actionAddFile, SIGNAL(triggered()), this, SLOT(slotAddFile()));
    const QIcon iconAddDir = QIcon("./images/Aggiungi_Cartella-50.png");
    actionAddDir->setIcon(iconAddDir);
    connect(actionAddDir, SIGNAL(triggered()), this, SLOT(slotAddDirectory()));
    toolBar->addAction(actionAddDir);
    const QIcon iconRename = QIcon("./images/Modifica_proprietà-50.png");
    actionRename->setIcon(iconRename);
    connect(actionRename, SIGNAL(triggered()), this, SLOT(slotRename()));
    toolBar->addAction(actionRename);
    const QIcon iconCopy = QIcon("./images/Copia-50.png");
    actionCopy->setIcon(iconCopy);
    toolBar->addAction(actionCopy);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(slotCopy()));
    const QIcon iconPaste = QIcon("./images/Incolla-50.png");
    actionPaste->setIcon(iconPaste);
    toolBar->addAction(actionPaste);
    actionPaste->setDisabled(true);
    connect(actionPaste, SIGNAL(triggered()), this, SLOT(slotPaste()));
    const QIcon iconShare = QIcon("./images/Condividi-48.png");
    actionShare->setIcon(iconShare);
    toolBar->addAction(actionShare);
    connect(actionShare, SIGNAL(triggered()), this, SLOT(slotShare()));
    const QIcon iconDelete = QIcon("images/Elimina-50.png");
    actionDelete->setIcon(iconDelete);
    toolBar->addAction(actionDelete);
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(slotDelete()));
    //widget usato per spostare le icone sul fondo
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    spacerWidget->setVisible(true);
    toolBar->addWidget(spacerWidget);
    const QIcon iconAccountInformation = QIcon("./images/informazioniAccount.png");
    actionAccountInformation->setIcon(iconAccountInformation);
    toolBar->addAction(actionAccountInformation);
    connect(actionAccountInformation, SIGNAL(triggered()), this, SLOT(slotAccountInformation()));
    const QIcon iconSettings = QIcon("./images/Impostazioni-50.png");
    actionSettings->setIcon(iconSettings);
    toolBar->addAction(actionSettings);
    connect(actionSettings, SIGNAL(triggered()), this, SLOT(slotSettings()));
    const QIcon iconLogout = QIcon("./images/logout-50.png");
    actionLogout->setIcon(iconLogout);
    toolBar->addAction(actionLogout);
    connect(actionLogout, SIGNAL(triggered()), this, SLOT(slotLogout()));
    addToolBar(Qt::RightToolBarArea, toolBar);
}
void cloudView::slotSettings() {
    changeInformationDialog = new myChangeInformationDialog(this, model->getUserLoggedEmail(), model->getUserLoggedName(), model->getUserLoggedLastName(), model->completeSearch());
    changeInformationDialog->exec();
    delete changeInformationDialog;
}
void cloudView::slotConfirmSettings() {
    try {
        model->changeAccountInformation(changeInformationDialog->getNewEmail(), changeInformationDialog->getNewName(),
                                        changeInformationDialog->getNewLastName(), changeInformationDialog->getPassword(),
                                        changeInformationDialog->getNewPassword1(), changeInformationDialog->getNewPassword2());
        QMessageBox::information(this, "Modifiche effettuate", "Le modifiche al tuo account sono state apportate con successo");
    }catch (feedback feed){
        QMessageBox::critical(this, "Errore", QString::fromStdString(feed));
    }

}
void cloudView::slotLogout(){
    if(QMessageBox::Ok == QMessageBox::question(this, "Logout", "Vuoi davvero effettuare il logout?", QMessageBox::Cancel|QMessageBox::Ok)){
        setWidgetLogin();
        model->logout();
    }
}
void cloudView::slotShare() {
    fileSystemNode* shareNode = userWidget->getTableFileSystemPointer();
    if(!shareNode){
        QMessageBox::critical(this, "Errore", "Attenzione, non è stato selezionato alcun elemento");
    }
    else{
        QDialog* input = new QDialog(this);
        input->setWindowTitle("Email destinatario");
        QVBoxLayout* layout = new QVBoxLayout(input);
        QLineEdit* email = new QLineEdit(input);
        QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, input);
        connect(buttonBox, SIGNAL(accepted()), input, SLOT(accept()));
        connect(buttonBox, SIGNAL(rejected()), input, SLOT(reject()));
        QLabel* label = new QLabel("Inserisci l'email dell'utente:", input);
        layout->addWidget(label);
        layout->addWidget(email);
        layout->addWidget(buttonBox);
        input->setLayout(layout);
        input->setMinimumSize(width()/5, 0);
        input->exec();
        bool findUser = false;
        while(input->result() == QDialog::Accepted && !findUser){
            if(email->text().isEmpty()){
                QMessageBox::critical(this, "Errore", "Attenzione, l'email non può essere vuota");
            }
            else if(email->text().toStdString() == model->getUserLoggedEmail()){
                QMessageBox::critical(this, "Errore", "Attenzione, impossibile condividere un file con se stessi");
            }
            else if(model->existUser(email->text().toStdString())){
                findUser = true;
                string mail = email->text().toStdString();
                input->setWindowTitle("Nome dell'elemento condiviso");
                label->setText("Inserisci il nuovo nome dell'elemento condiviso");
                email->setText(QString::fromStdString(shareNode->getName()));
                bool share = false;
                input->exec();
                while(input->result() == QDialog::Accepted && !share){
                    if(email->text().isEmpty()){
                        QMessageBox::critical(this, "Errore", "Attenzione, il nome non può essere vuoto");
                    }
                    else{
                        try {
                            model->share(mail, shareNode, email->text().toStdString());
                            share = true;
                            QMessageBox::information(this, "Condivisione avvenuta", "La condivisione con " + QString::fromStdString(mail) + " dell'elemento scelto è stata effetuata con successo");
                        }
                        catch(feedback feed){
                            QMessageBox::critical(this, "Errore", QString::fromStdString(feed));
                        }
                    }
                    if(!share){
                        input->exec();
                    }
                }
            }
            else{
                QMessageBox::critical(this, "Errore", "Attenzione, l'utente specificato non esiste");
            }
            if(!findUser){
                input->exec();
            }
        }
        delete input;
    }
}
void cloudView::slotCopy() {
    fileSystemNode* f = userWidget->getTableFileSystemPointer();
    if(!f){
        QMessageBox::critical(this, "Errore", "Attenzione, non è stato selezionato nessun elemento");
    }
    else{
        userWidget->setActionPaste(true);
        moveNode = f;
        actionPaste->setDisabled(false);
    }
}
void cloudView::slotPaste() {
    if(!moveNode){
        QMessageBox::critical(this, "Errore", "Attenzione, non è stato selezionato alcun elemento da spostare");
    }
    else{
        if(moveNode->contains(model->getCurrentDirectory())){
            QMessageBox::critical(this, "Errore", "Attenzione, non è possibile incollare un elemento all'interno di esso");
        }
        else{
            fileSystemNode* newElement = 0;
            if(model->getCurrentDirectory()->existName(moveNode->getName())){
                QDialog* input = new QDialog(this);
                input->setWindowTitle("Rinomina l'elemento");
                input->setMinimumSize(width()/5, 0);
                QVBoxLayout* layout = new QVBoxLayout(input);
                QLineEdit* name = new QLineEdit(input);
                QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, input);
                connect(buttonBox, SIGNAL(accepted()), input, SLOT(accept()));
                connect(buttonBox, SIGNAL(rejected()), input, SLOT(reject()));
                layout->addWidget(new QLabel("Nella cartella corrente è gia presente un elemento con lo stesso nome\nNuovo nome:", input));
                layout->addWidget(name);
                layout->addWidget(buttonBox);
                input->setLayout(layout);
                bool correctName = false;
                input->exec();
                while(input->result() == QDialog::Accepted && !correctName){
                    if(name->text().isEmpty()){
                        QMessageBox::critical(this, "Errore", "Attenzione, il nome non può essere vuoto");
                    }
                    else if(!model->getCurrentDirectory()->existName(name->text().toStdString())){
                        correctName = true;
                        newElement = moveNode->clone(0);
                        newElement->rename(name->text().toStdString());
                        int pos = model->getCurrentDirectory()->addNode(newElement);
                        userWidget->addQTreeWidgetItem(newElement, pos);
                        userWidget->createTableWidget(model->getCurrentDirectory()->openDirectory());
                    }
                    if(!correctName){
                        input->exec();
                    }
                }
                delete input;
            }
            else{
                newElement = moveNode->clone(0);
                userWidget->addQTreeWidgetItem(newElement, newElement->setParent(model->getCurrentDirectory()));
                userWidget->createTableWidget(model->getCurrentDirectory()->openDirectory());
            }
        }
    }
}
void cloudView::slotRename() {
    fileSystemNode* renameNode = userWidget->getTableFileSystemPointer();
    if (!renameNode){
        QMessageBox::critical(this, "Errore", "Attenzione, non è stato selezionato alcun elemento");
    }
    else{
        QDialog* input = new QDialog(this);
        input->setWindowTitle("Rinomina");
        QVBoxLayout* layout = new QVBoxLayout(input);
        QLineEdit* name = new QLineEdit(input);
        QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, input);
        connect(buttonBox, SIGNAL(accepted()), input, SLOT(accept()));
        connect(buttonBox, SIGNAL(rejected()), input, SLOT(reject()));
        layout->addWidget(new QLabel("Nuovo nome:", input));
        layout->addWidget(name);
        layout->addWidget(buttonBox);
        input->setLayout(layout);
        input->setMinimumSize(width()/5, 0);
        bool correctName = false;
        input->exec();
        while(input->result() == QDialog::Accepted && !correctName){
            if(name->text().isEmpty()){
                QMessageBox::critical(this, "Errore", "Attenzione, il nome non può essere vuoto");
            }
            else if(!model->getCurrentDirectory()->existName(name->text().toStdString())){
                correctName = true;
                userWidget->removeQTreeWidgetItem(renameNode);
                int pos = renameNode->rename(name->text().toStdString());
                userWidget->addQTreeWidgetItem(renameNode, pos);
                userWidget->createTableWidget(model->getCurrentDirectory()->openDirectory());
            }
            else{
                QMessageBox::critical(this, "Errore", "Attenzione, il nuovo nome scelto è gia presente nella directory corrente");
            }
            if(!correctName){
                input->exec();
            }
        }
        delete input;
    }
}
void cloudView::openDirectory(directoryNode* dir) {
    try {
        directoryNode* directory = model->navigateTree(dir);
        if(model->isUserDirectory()){
            userWidget->createUserTableWidget(model->getUsers());
        }
        else{
            actionAddFile->setDisabled(false);
            actionAddDir->setDisabled(false);
            userWidget->createTableWidget((directory->openDirectory()));
        }
        if(model->isTheTop()){
            actionLevelUp->setDisabled(true);
        }
        else{
            actionLevelUp->setDisabled(false);
        }
        disableActions();
    }
    catch(feedback feed){
        QMessageBox::critical(this, "Errore", QString::fromStdString(feed));
    }
}
void cloudView::slotDelete() {
    fileSystemNode* f = userWidget->getTableFileSystemPointer();
    if(model->isUserDirectory()){
        if(f){
            if(QMessageBox::Ok == QMessageBox::question(this, "Elimina", "Vuoi davvero eliminare l'utente selezionato?", QMessageBox::Cancel|QMessageBox::Ok)){
                model->deleteUser(f);
                userWidget->removeQTreeWidgetItem(f);
                userWidget->createTableWidget(model->getCurrentDirectory()->openDirectory());
                disableActions();
            }
        }
        else{
            QMessageBox::critical(this, "Errore", "Attenzione, non è stato selezionato nessun utente da eliminare");
        }
    }
    else{
        if(f){
            if(QMessageBox::Ok == QMessageBox::question(this, "Elimina", "Vuoi davvero eliminare l'elemento selezionato?", QMessageBox::Cancel|QMessageBox::Ok)){
                try {
                    model->deleteNode(f);
                    userWidget->removeQTreeWidgetItem(f);
                    userWidget->createTableWidget(model->getCurrentDirectory()->openDirectory());
                    disableActions();
                }catch (feedback feed){
                    QMessageBox::critical(this, "Errore", QString::fromStdString(feed));
                }
            }
        }
        else{
            QMessageBox::critical(this, "Errore", "Attenzione, non è stato selezionato nessun elemento");
        }
    }
}
void cloudView::slotLevelUp() {
    if(!model->isTheTop()){
        directoryNode* dir = model->getParent();
        if(model->isUserDirectory()){
            userWidget->createUserTableWidget(model->getUsers());
        }
        else{
            userWidget->createTableWidget(dir->openDirectory());
        }
        userWidget->QTreeWidgetItemLevelUp();
        actionLevelUp->setDisabled(model->isTheTop());
        disableActions();
        if(model->adminIsLogged()){
            disableAdminActions();
        }
    }
    else{
        QMessageBox::critical(this, "Errore", "Attenzione, ti trovi già al livello massimo");
    }
}
void cloudView::slotAddFile() {
    QDialog* input = new QDialog(this);
    input->setWindowTitle("Nuovo file");
    QVBoxLayout* layout = new QVBoxLayout(input);
    QLineEdit* name = new QLineEdit(input);
    QLineEdit* size = new QLineEdit(input);
    QComboBox* comboBox = new QComboBox(input);
    comboBox->addItem("byte", "byte");
    comboBox->addItem("KB", "KB");
    comboBox->addItem("MB", "MB");
    comboBox->addItem("GB", "GB");
    comboBox->addItem("TB", "TB");
    layout->addWidget(new QLabel("Nome file:", input));
    layout->addWidget(name);
    layout->addWidget(new QLabel("Dimensione file:", input));
    QWidget* widgetSize = new QWidget(input);
    QHBoxLayout* horizontalLayout = new QHBoxLayout(widgetSize);
    QSizePolicy left(QSizePolicy::Preferred, QSizePolicy::Preferred);
    left.setHorizontalStretch(3);
    QSizePolicy right(QSizePolicy::Preferred, QSizePolicy::Preferred);
    right.setHorizontalStretch(1);
    size->setSizePolicy(left);
    comboBox->setSizePolicy(right);
    horizontalLayout->addWidget(size);
    horizontalLayout->addWidget(comboBox);
    widgetSize->setLayout(horizontalLayout);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(widgetSize);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, input);
    connect(buttonBox, SIGNAL(accepted()), input, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), input, SLOT(reject()));
    layout->addWidget(buttonBox);
    input->setMinimumSize(width()/5, 0);
    if(input->exec() == QDialog::Accepted && !size->text().isEmpty() && !name->text().isEmpty()){
        fileNode* f = 0;
        try {
            string nameFile = name->text().toStdString();
            byteSize sizeFile(size->text().toStdString() + " " + comboBox->currentText().toStdString());
            f = new fileNode(nameFile, sizeFile);
            int pos = model->addNode(f);
            userWidget->createTableWidget(model->getCurrentDirectory()->openDirectory());
            userWidget->addQTreeWidgetItem(f, pos);
            disableActions();
        }catch (feedback feed){
            delete f;
            QMessageBox::critical(this, "Attenzione", QString::fromStdString(feed));
        }
    }
    else if (input->result() == QDialog::Accepted){
        QMessageBox::critical(this, "Attenzione", "I valori inseriti sono errati");
    }
    delete input;
}
void cloudView::slotAddDirectory() {
    QDialog* input = new QDialog(this);
    input->setWindowTitle("Nuova cartella");
    QVBoxLayout* layout = new QVBoxLayout(input);
    QLineEdit* name = new QLineEdit(input);
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, input);
    connect(buttonBox, SIGNAL(accepted()), input, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), input, SLOT(reject()));
    layout->addWidget(new QLabel("Nome cartella:", input));
    layout->addWidget(name);
    layout->addWidget(buttonBox);
    input->setLayout(layout);
    input->setMinimumSize(width()/5, 0);
    if (input->exec() == QDialog::Accepted && !name->text().isEmpty()){
        directoryNode *newDir = new directoryNode(name->text().toStdString());
        try {
            int pos = model->addNode(newDir);
            userWidget->createTableWidget(model->getCurrentDirectory()->openDirectory());
            userWidget->addQTreeWidgetItem(newDir, pos);
            disableActions();
        } catch (feedback feed) {
            delete newDir;
            QMessageBox::critical(this, "Errore", QString::fromStdString(feed));
        }
    }
    else if(input->result() == QDialog::Accepted){
        QMessageBox::critical(this, "Attenzione", "Il nome non può essere vuoto");
    }
    delete input;
}
void cloudView::slotEnableActions() {
    if(model->isUserDirectory()){
        disableAdminActions();
    }
    else{
        actionCopy->setDisabled(false);
        actionShare->setDisabled(false);
        actionRename->setDisabled(false);
    }
    actionDelete->setDisabled(false);
}
void cloudView::slotAddUser(){
    int type = widgetLogin->getComboIndex();
    user* newUser = 0;
    try{
        if(type == 0){
            newUser = new standardUser(widgetLogin->getName(), widgetLogin->getLastName(), widgetLogin->getEmail(), widgetLogin->getPassword());
            model->addUser(newUser);
            model->createUserRoot(newUser);
        }
        else if(type == 1){
            newUser = new premiumUser(widgetLogin->getName(), widgetLogin->getLastName(), widgetLogin->getEmail(), widgetLogin->getPassword());
            model->addUser(newUser);
            model->createUserRoot(newUser);
        }
        model->login(widgetLogin->getEmail(), widgetLogin->getPassword());
        delete newUser;
        setWidgetUser();
    }catch(feedback feed){
        delete newUser;
        QMessageBox::critical(this, "Errore", QString::fromStdString(feed));
    }
}
void cloudView::slotSearch() {
    try{
        searchNode searchInfo = userWidget->getSearchInformation();
        vector<fileSystemNode*> searchNodes = model->search(searchInfo);
        searchDialog = new QDialog(this);
        searchDialog->setFixedHeight(height()/3*2);
        searchDialog->setFixedWidth(width()/3*2);
        searchDialog->setWindowTitle("Risultato della ricerca");
        searchTable = new QTableWidget(0, 3, searchDialog);
        QGridLayout* searchLayout = new QGridLayout(searchDialog);
        searchLayout->addWidget(searchTable, 1, 1, 1, 1);
        QStringList listaLabel;
        listaLabel.append("Nome");
        listaLabel.append("Dimensione");
        listaLabel.append("Condiviso da");
        searchTable->setHorizontalHeaderLabels(listaLabel);
        searchTable->setShowGrid(false);
        searchTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //searchTable->setContextMenuPolicy(Qt::CustomContextMenu);
        searchTable->verticalHeader()->hide();
        searchTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        searchTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        searchTable->setSelectionMode(QAbstractItemView::SingleSelection);
        connect(searchTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(slotOpenSearchNode(int, int)));
        searchTable->setRowCount(0);
        for(auto it = searchNodes.begin(); it != searchNodes.end(); ++it){
            searchTable->insertRow(searchTable->rowCount());
            searchTable->setVerticalHeaderItem(searchTable->rowCount() -1, new myQTableWidgetItem(*it));
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString((*it)->getName()));
            item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
            dynamic_cast<directoryNode*>(*it) ? item->setIcon(QIcon("./images/Folder-50.png")) : item->setIcon(QIcon("./images/File-50.png"));
            searchTable->setItem(searchTable->rowCount() -1, 0, item);
            item = new QTableWidgetItem(QString::fromStdString((*it)->getSize()));
            item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
            searchTable->setItem(searchTable->rowCount() -1, 1, item);
            fileSystemNodeShared* shareNode = dynamic_cast<fileSystemNodeShared*>(*it);
            item = new QTableWidgetItem(QString::fromStdString(shareNode ? shareNode->sharedBy() : ""));
            item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
            searchTable->setItem(searchTable->rowCount() -1, 2, item);
        }
        searchDialog->exec();
        delete searchDialog;
    }
    catch(feedback feed){
        QMessageBox::critical(this, "Errore", QString::fromStdString(feed));
    }
}
void cloudView::slotUpgradeAccount() {
    if(QMessageBox::Ok == QMessageBox::question(this, "Passa a premium", "Desideri sbloccare le funzionalità premium? Per rendere effettive le modifiche "
            "verrai scollegato dal tuo account e dovrai rieffettuare il login", QMessageBox::Cancel|QMessageBox::Ok)){
        model->upgradeAccount();
        changeInformationDialog->close();
        model->logout();
        setWidgetLogin();
    }
}
void cloudView::slotOpenSearchNode(int row, int) {
    myQTableWidgetItem* item = dynamic_cast<myQTableWidgetItem*>(searchTable->verticalHeaderItem(row));
    fileSystemNode* node = item->getNodePointer();
    openDirectory(dynamic_cast<directoryNode*>(node->getParent()));
    userWidget->setQTreeWidgetCurrentItem(node->getParent());
    searchDialog->close();
}
void cloudView::disableActions() {
    actionDelete->setDisabled(true);
    actionCopy->setDisabled(true);
    actionShare->setDisabled(true);
    actionRename->setDisabled(true);
}
void cloudView::restore(){
    auto fileName = QFileDialog::getOpenFileName(0, "Seleziona il file di backup da caricare", "./backup/backup.xml", "Xml file (*.xml)");
    if(fileName != ""){
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QFile::Text)){
            throw feedback(0, "Restore non riuscito, impossibile leggere il file");
        }
        QXmlStreamReader xmlReader(&file);
        xmlReader.readNextStartElement();
        if(xmlReader.name() != "cloud"){//se il file non è corretto creo un albero composto solo da una root e un utente admin di default
            file.close();
            throw feedback(0, "Restore non riuscito, il file selezionato è corrotto");
        }
        xmlReader.readNextStartElement();
        if(xmlReader.name() != "users"){
            file.close();
            throw feedback(0, "Restore non riuscito, il file selezionato è corrotto");
        }
        xmlReader.readNextStartElement();
        while(xmlReader.name() != "users"){
            if(!xmlReader.isEndElement()){
                QXmlStreamAttributes attributes = xmlReader.attributes();
                string name = attributes.value("name").toString().toStdString();
                string lastName = attributes.value("lastName").toString().toStdString();
                string email = attributes.value("email").toString().toStdString();
                string password = attributes.value("password").toString().toStdString();
                if(attributes.value("type") == "admin"){
                    admin* a = new admin(name, lastName, email, password);
                    model->setAdmin(a);
                    delete a;
                }
                else if(attributes.value("type") == "standardUser"){
                    standardUser* s = new standardUser(name, lastName, email, password);
                    model->addUser(s);
                    delete s;
                }
                else if(attributes.value("type") == "premiumUser"){
                    premiumUser* p = new premiumUser(name, lastName, email, password);
                    model->addUser(p);
                    delete p;
                }
            }
            xmlReader.readNextStartElement();
        }
        xmlReader.readNextStartElement();
        if(xmlReader.name() != "tree"){
            file.close();
            throw feedback(0, "Restore non riuscito, il file selezionato è corrotto");
        }
        xmlReader.readNextStartElement();
        fileSystemNode* root;
        try {
            root = createTree(xmlReader);
        }catch (feedback feed){
            file.close();
            throw feed;
        }
        try{
            model->setRoot(root);
        }catch (feedback){
            file.close();
            throw feedback(0, "Restore non riuscito, non è presente una root");
        }
        try {
            model->setUserRoot();//potrebbe sollevare eccezioni
        }catch(feedback feed){
            file.close();
            throw feed;
        }
    }
    else {
        throw feedback(0, "Restore non riuscito, non è stato selezionato alcun file");
    }
}
void cloudView::slotDeleteAccount() {
    if(model->adminIsLogged()){
        QMessageBox::critical(this, "Impossibile eliminare account", "Attenzione, è impossibile rimuovere l'account amministratore");
    }
    else{
        if(QMessageBox::Ok == QMessageBox::question(this, "Elimina account", "Vuoi davvero eliminare il tuo account? "
                "Tutti i tuoi file verranno eliminati", QMessageBox::Cancel|QMessageBox::Ok)){
            changeInformationDialog->close();
            model->deleteUser();
            setWidgetLogin();
        }
    }
}
void cloudView::slotAccountInformation() {
    QMessageBox::information(this, "Informazioni account", QString::fromStdString(model->getAccountInformation()));
}
fileSystemNode* cloudView::createTree(QXmlStreamReader &xmlReader) const{
    if(xmlReader.name() == "node" && !xmlReader.isEndElement()){
        QXmlStreamAttributes attributes = xmlReader.attributes();
        string type = attributes.value("type").toString().toStdString();
        if(type == "fileNode" || type == "fileNodeShared"){
            string name = attributes.value("name").toString().toStdString();
            unsigned long long int size = attributes.value("size").toULongLong();
            fileNode *file = new fileNode(name, byteSize(size));
            xmlReader.readNextStartElement();
            if(type == "fileNode"){
                return file;
            }
            return new fileNodeShared(*file, attributes.value("sharedBy").toString().toStdString());
        }
        if(type == "directoryNode" || type == "directoryNodeShared" || type == "rootNode"){
            directoryNode *dir = 0;
            string name = attributes.value("name").toString().toStdString();
            if(type == "directoryNode" || type == "directoryNodeShared"){
                dir = new directoryNode(name);
                if(type == "directoryNodeShared"){
                    dir = new directoryNodeShared(*dir, attributes.value("sharedBy").toString().toStdString());
                }
            }
            else if(type == "rootNode"){
                string userName = attributes.value("userName").toString().toStdString();
                dir = new rootNode(name, userName);
            }
            xmlReader.readNextStartElement();
            while(!xmlReader.isEndElement()){
                dir->addNode(createTree(xmlReader));//oltre ad aggiungere il nodo addNode si preoccupa di settare correttamente il padre
                xmlReader.readNextStartElement();
            }
            return dir;
        }
    }
    throw feedback(0, "Restore non riuscito, file system compromesso");
}
void cloudView::slotBackup() {
    try{
        backup();
    }catch (feedback feed){
        QMessageBox::critical(this, "Errore", QString::fromStdString(feed));
    }

}
void cloudView::backup() const{
    QString fileName = QFileDialog::getSaveFileName(0, "Seleziona il file dove effettuare il backup", "./backup/backup.xml", "Xml file (*.xml)");
    if(fileName != ""){
        list<user*> users = model->getUsers();
        QFile fileXml(fileName);
        fileXml.open(QIODevice::WriteOnly);
        QXmlStreamWriter xmlWriter(&fileXml);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("cloud");
        xmlWriter.writeStartElement("users");
        for (list<user*>::const_iterator it = users.begin(); it != users.end(); ++it) {
            xmlWriter.writeStartElement("user");
            xmlWriter.writeAttribute("type", QString::fromStdString((*it)->getType()));
            xmlWriter.writeAttribute("email", QString::fromStdString((*it)->getEmail()));
            xmlWriter.writeAttribute("name", QString::fromStdString((*it)->getName()));
            xmlWriter.writeAttribute("lastName", QString::fromStdString((*it)->getLastName()));
            xmlWriter.writeAttribute("password", QString::fromStdString((*it)->getPassword()));
            xmlWriter.writeEndElement();//user
            delete *it;
        }
        xmlWriter.writeEndElement();//users
        xmlWriter.writeStartElement("tree");
        writeTreeXml(xmlWriter, model->getTree());
        xmlWriter.writeEndElement();//tree
        xmlWriter.writeEndElement();//cloud
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        fileXml.close();
    }
    else{
        throw feedback(0, "Backup non riuscito, non è stato selezionato alcun file");
    }
}
void cloudView::disableAdminActions() const{
    if(model->isUserDirectory()){
        actionRename->setDisabled(true);
        actionPaste->setDisabled(true);
        actionShare->setDisabled(true);
        actionAddDir->setDisabled(true);
        actionAddFile->setDisabled(true);
    }
}
void cloudView::writeTreeXml(QXmlStreamWriter& xmlWriter, fileSystemNode* node) const {
    xmlWriter.writeStartElement("node");
    xmlWriter.writeAttribute("name", QString::fromStdString(node->getName()));
    if(dynamic_cast<fileNode*> (node)){
        xmlWriter.writeAttribute("type", QString::fromStdString(node->getType()));
        xmlWriter.writeAttribute("size", QString::fromStdString(std::to_string(node->getSize().getNByte())));
        fileSystemNodeShared* shareNode = dynamic_cast<fileSystemNodeShared*>(node);
        xmlWriter.writeAttribute("sharedBy", QString::fromStdString(shareNode ? shareNode->sharedBy() : ""));
    }
    else {
        directoryNode *dir = dynamic_cast<directoryNode*> (node);
        list<fileSystemNode*> subNodes = dir->openDirectory();
        xmlWriter.writeAttribute("type", QString::fromStdString(node->getType()));
        fileSystemNodeShared* shareNode = dynamic_cast<fileSystemNodeShared*>(node);
        xmlWriter.writeAttribute("sharedBy", QString::fromStdString(shareNode ? shareNode->sharedBy() : ""));
        rootNode* root = dynamic_cast<rootNode*>(node);
        xmlWriter.writeAttribute("userName", QString::fromStdString(root ? root->getUserName() : ""));
        for (list<fileSystemNode*>::const_iterator it = subNodes.begin(); it != subNodes.end(); ++it) {
            writeTreeXml(xmlWriter, *it);
        }
    }
    xmlWriter.writeEndElement();//node
}
cloudView::~cloudView() {}
