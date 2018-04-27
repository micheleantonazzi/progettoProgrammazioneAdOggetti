//
// Created by michele on 03/04/17.
//

#include <QtWidgets/QHeaderView>
#include <QList>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include "myUserWidget.h"
#include "myQTableWidgetItem.h"
#include "QLabel"
#include "model/fileSystemNodeShared.h"

myUserWidget::myUserWidget(QWidget *parent, bool b) : QWidget(parent), completeSearch(b), treeWidget(new QTreeWidget(this)),
                                              tableWidget(new QTableWidget(0, 3, this)), enablePaste(false),
                                              searchLine(new QLineEdit(this)), checkBoxFile(new QCheckBox("File", this)),
                                              checkBoxDirectory(new QCheckBox("Cartelle", this)), minSizeLine(new QLineEdit(this)),
                                              maxSizeLine(new QLineEdit(this)), minSizeCombo(new QComboBox(this)),
                                              maxSizeCombo(new QComboBox(this)), searchButton(new QPushButton("", this)){
    QGridLayout* gridLayout = new QGridLayout(this);
    QWidget* searchWidget = new QWidget(this);
    QGridLayout* searchLayout = new QGridLayout(searchWidget);
    searchLayout->addWidget(new QLabel("Cerca:"), 0, 1, 1, 1);
    searchLayout->addWidget(searchLine, 1, 1, 1, 1);
    connect(searchLine, SIGNAL(textChanged(const QString&)), this, SLOT(slotEnableSearchButton()));
    searchLayout->addWidget(new QLabel("Cosa vuoi cercare?", searchWidget), 0, 2, 1, 2);
    searchLayout->addWidget(checkBoxFile, 1, 2, 1, 1);
    searchLayout->addWidget(checkBoxDirectory, 1, 3, 1, 1);
    minSizeCombo->addItem("byte", "byte");
    minSizeCombo->addItem("KB", "KB");
    minSizeCombo->addItem("MB", "MB");
    minSizeCombo->addItem("GB", "GB");
    minSizeCombo->addItem("TB", "TB");
    maxSizeCombo->addItem("byte", "byte");
    maxSizeCombo->addItem("KB", "KB");
    maxSizeCombo->addItem("MB", "MB");
    maxSizeCombo->addItem("GB", "GB");
    maxSizeCombo->addItem("TB", "TB");
    searchLayout->addWidget(new QLabel(!completeSearch ? "Intervallo di ricerca: (disponibile agli utenti premium)" : "Intervallo di ricerca:" , this), 0, 4, 1, 4);
    searchLayout->addWidget(minSizeLine, 1, 4, 1, 1);
    searchLayout->addWidget(minSizeCombo, 1, 5, 1, 1);
    searchLayout->addWidget(maxSizeLine, 1, 6, 1, 1);
    searchLayout->addWidget(maxSizeCombo, 1, 7, 1, 1);
    connect(minSizeLine, SIGNAL(textChanged(const QString&)), this, SLOT(slotEnableSearchButton()));
    connect(maxSizeLine, SIGNAL(textChanged(const QString&)), this, SLOT(slotEnableSearchButton()));
    connect(checkBoxFile, SIGNAL(stateChanged(int)), this, SLOT(slotEnableSearchButton()));
    connect(checkBoxDirectory, SIGNAL(stateChanged(int)), this, SLOT(slotEnableSearchButton()));
    if(!completeSearch){
        minSizeCombo->setDisabled(true);
        minSizeLine->setDisabled(true);
        maxSizeCombo->setDisabled(true);
        maxSizeLine->setDisabled(true);
    }
    const QIcon iconSearch = QIcon("./images/Ricerca-50.png");
    searchButton->setIcon(iconSearch);
    searchButton->setMinimumHeight(searchLine->height() * 2);
    searchButton->setMinimumWidth(searchButton->height());
    searchButton->setDisabled(true);
    connect(searchButton, SIGNAL(clicked()), parentWidget(), SLOT(slotSearch()));
    searchLayout->addWidget(searchButton, 0, 8, 2, 1);
    searchLayout->setHorizontalSpacing(15);
    searchWidget->setLayout(searchLayout);
    searchWidget->setObjectName("searchWidget");
    gridLayout->addWidget(treeWidget, 0, 1, 2, 3);
    gridLayout->addWidget(searchWidget, 0, 4, 1, 6);
    gridLayout->addWidget(tableWidget, 1, 4, 1, 6);
    QStringList listaLabel;
    listaLabel.append("");
    listaLabel.append("");
    listaLabel.append("");
    tableWidget->setHorizontalHeaderLabels(listaLabel);
    tableWidget->horizontalHeader()->setHighlightSections(false);
    tableWidget->setShowGrid(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    tableWidget->verticalHeader()->hide();
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotTableOpenDirectory(QModelIndex)));
    connect(this, SIGNAL(openDirectory(directoryNode*)), parentWidget(), SLOT(openDirectory(directoryNode*)));
    connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slotTreeOpenDirectory(QTreeWidgetItem*, int)));
    connect(tableWidget, SIGNAL(cellClicked(int, int)), parentWidget(), SLOT(slotEnableActions()));
}
void myUserWidget::createTableWidget(list<fileSystemNode*> l) const {
    tableWidget->horizontalHeaderItem(0)->setText("Nome");
    tableWidget->horizontalHeaderItem(1)->setText("Dimensione");
    tableWidget->horizontalHeaderItem(2)->setText("Condiviso da");
    tableWidget->setRowCount(0);
    for(auto it = l.begin(); it != l.end(); ++it){
        tableWidget->insertRow(tableWidget->rowCount());
        tableWidget->setVerticalHeaderItem(tableWidget->rowCount() -1, new myQTableWidgetItem(*it));
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString((*it)->getName()));
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        dynamic_cast<directoryNode*>(*it) ? item->setIcon(QIcon("./images/Folder-50.png")) : item->setIcon(QIcon("./images/File-50.png"));
        tableWidget->setItem(tableWidget->rowCount() -1, 0, item);
        item = new QTableWidgetItem(QString::fromStdString((*it)->getSize()));
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount() -1, 1, item);
        fileSystemNodeShared* shareNode = dynamic_cast<fileSystemNodeShared*>(*it);
        item = new QTableWidgetItem(QString::fromStdString(shareNode ? shareNode->sharedBy() : ""));
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount() -1, 2, item);
    }
}
void myUserWidget::createUserTableWidget(list<user*> users) const {
    tableWidget->horizontalHeaderItem(0)->setText("Email utente");
    tableWidget->horizontalHeaderItem(1)->setText("Nome utente");
    tableWidget->horizontalHeaderItem(2)->setText("Spazio occupato");
    tableWidget->setRowCount(0);
    for(auto it = ++users.begin(); it != users.end(); ++it){
        tableWidget->insertRow(tableWidget->rowCount());
        tableWidget->setVerticalHeaderItem(tableWidget->rowCount() -1, new myQTableWidgetItem((*it)->getRoot()));
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString((*it)->getEmail()));
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        item->setIcon(QIcon("./images/Utente-50.png"));
        tableWidget->setItem(tableWidget->rowCount() -1, 0, item);
        item = new QTableWidgetItem(QString::fromStdString((*it)->getUserName()));
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount() -1, 1, item);
        std::pair<int, int> subNodes = (*it)->getRoot()->countNodes();
        item = new QTableWidgetItem(QString::fromStdString((*it)->getRoot()->getSize()));
        item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount() -1, 2, item);
    }
}
void myUserWidget::QTreeWidgetItemLevelUp() {
    if(currentTreeWidgetItem && currentTreeWidgetItem->parent()){
        currentTreeWidgetItem = currentTreeWidgetItem->parent();
    }
}
void myUserWidget::slotTableOpenDirectory(const QModelIndex &index) {
    fileSystemNode* f = (static_cast<myQTableWidgetItem*>(tableWidget->verticalHeaderItem(index.row())))->getNodePointer();
    directoryNode* dir = dynamic_cast<directoryNode*> (f);
    if(dir){
        QTreeWidgetItem* old = currentTreeWidgetItem;
        QList<QTreeWidgetItem*> children = currentTreeWidgetItem->takeChildren();
        auto it = children.begin();
        while(it != children.end()) {
            if((static_cast<myQTreeWidgetItem*>(*it))->getNode() == dir){
                currentTreeWidgetItem = *it;
                it = children.end();
            }
            else{
                ++it;
            }
        }
        old->addChildren(children);
        emit openDirectory(dir);
    }
}
void myUserWidget::slotTreeOpenDirectory(QTreeWidgetItem* item, int) {
    myQTreeWidgetItem* myItem = dynamic_cast<myQTreeWidgetItem*>(item);
    if(myItem){
        directoryNode* dir = dynamic_cast<directoryNode*>(myItem->getNode());
        if (dir){
            item->setExpanded(false);
            currentTreeWidgetItem = item;
            emit openDirectory(dir);
        }
    }
}
void myUserWidget::createTreeWidget(directoryNode* root){
    QList<QTreeWidgetItem*> children = treeWidget->invisibleRootItem()->takeChildren();
    for(auto it = children.begin(); it != children.end(); ++it){
        delete *it;
    }
    treeWidget->setColumnCount(1);
    treeWidget->setHeaderLabel("Cartelle e file");
    myQTreeWidgetItem* item = new myQTreeWidgetItem(treeWidget->invisibleRootItem(), root);
    item->setIcon(0, QIcon("./images/HDD-50.png"));
    item->setText(0, "Il mio archivio");
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    list<fileSystemNode*> subNodes = root->openDirectory();
    currentTreeWidgetItem = item;
    for(auto it = subNodes.begin(); it != subNodes.end(); ++it){
        createTree(*it, item);
    }
}
void myUserWidget::createUserTreeWidget(directoryNode* root){
    QList<QTreeWidgetItem*> children = treeWidget->invisibleRootItem()->takeChildren();
    for(auto it = children.begin(); it != children.end(); ++it){
        delete *it;
    }
    treeWidget->setColumnCount(1);
    treeWidget->setHeaderLabel("Utenti, cartelle e file");
    myQTreeWidgetItem* item = new myQTreeWidgetItem(treeWidget->invisibleRootItem(), root);
    item->setIcon(0, QIcon("./images/HDD-50.png"));
    item->setText(0, "Il mio archivio");
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    list<fileSystemNode*> subNodes = root->openDirectory();
    currentTreeWidgetItem = item;
    for(auto it = subNodes.begin(); it != subNodes.end(); ++it){
        myQTreeWidgetItem* subItem = new myQTreeWidgetItem(item, *it);
        subItem->setIcon(0, QIcon("./images/Utente-50.png"));
        subItem->setText(0, QString::fromStdString((*it)->getName()));
        subItem->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        list<fileSystemNode*> subNodes2 = (dynamic_cast<directoryNode*>(*it))->openDirectory();
        for(auto it2 = subNodes2.begin(); it2 != subNodes2.end(); ++it2){
            createTree(*it2, subItem);
        }
    }
}
void myUserWidget::createTree(fileSystemNode* subNode, QTreeWidgetItem *parent) const {
    directoryNode *directory = dynamic_cast<directoryNode *> (subNode);
    myQTreeWidgetItem* item = new myQTreeWidgetItem(parent, subNode);
    if (directory) {
        list<fileSystemNode*> nodes = directory->openDirectory();
        for (auto it = nodes.begin(); it != nodes.end(); ++it) {
            createTree(*it, item);
        }
        item->setIcon(0, QIcon("./images/Folder-50.png"));
    }
    else{
        item->setIcon(0, QIcon("./images/File-50.png"));
    }
    item->setText(0, QString::fromStdString(subNode->getName()));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
}
void myUserWidget::addQTreeWidgetItem(fileSystemNode *f, int pos) const {
    myQTreeWidgetItem* item = new myQTreeWidgetItem(0, f);
    directoryNode* dir = dynamic_cast<directoryNode*>(f);
    if(dir){
        item->setIcon(0, QIcon("./images/Folder-50.png"));
        auto subNodes = dir->openDirectory();
        for (auto it = subNodes.begin(); it != subNodes.end() ; ++it) {
            createTree(*it, item);
        }
    }
    else{
        item->setIcon(0, QIcon("./images/File-50.png"));
    }
    item->setText(0, QString::fromStdString(f->getName()));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    currentTreeWidgetItem->insertChild(pos, item); //insert children setta auotomaticamente il padre di item
}
void myUserWidget::slotEnableSearchButton() {
    searchButton->setDisabled(true);
    if(checkBoxDirectory->isChecked() || checkBoxFile->isChecked()){
        if(completeSearch){
            if((searchLine->text().isEmpty() && !minSizeLine->text().isEmpty() && !maxSizeLine->text().isEmpty()) ||
               (!searchLine->text().isEmpty() && minSizeLine->text().isEmpty() && maxSizeLine->text().isEmpty()) ||
               (!searchLine->text().isEmpty() && !minSizeLine->text().isEmpty() && !maxSizeLine->text().isEmpty()) ||
               (searchLine->text().isEmpty() && minSizeLine->text().isEmpty() && maxSizeLine->text().isEmpty())){
                searchButton->setDisabled(false);
            }
        }
        else {
            searchButton->setDisabled(false);
        }
    }
}
fileSystemNode* myUserWidget::getTableFileSystemPointer() const {
    QModelIndexList select = tableWidget->selectionModel()->selectedRows();
    if(select.isEmpty()){//se non ho selezionato nessuna riga nella tabella ritorno un puntatore nullo
        return 0;
    }
    myQTableWidgetItem* selectedItem = static_cast<myQTableWidgetItem*>(tableWidget->verticalHeaderItem(select.at(0).row()));
    return selectedItem->getNodePointer();
}
void myUserWidget::removeQTreeWidgetItem(fileSystemNode* f) const {
    QList<QTreeWidgetItem*> children = currentTreeWidgetItem->takeChildren();
    auto it = children.begin();
    while(it != children.end()) {
        if((static_cast<myQTreeWidgetItem*>(*it))->getNode() == f){
            it = children.erase(it);
            it = children.end();
        }
        else{
            ++it;
        }
    }
    currentTreeWidgetItem->addChildren(children);
}
void myUserWidget::showContextMenu(const QPoint& pos){
    fileSystemNode* f = getTableFileSystemPointer();
    QPoint globalPosition = tableWidget->mapToGlobal(pos);
    QMenu myMenu;
    const QIcon iconCopy = QIcon::fromTheme("copy", QIcon("./images/Copia-50.png"));
    QAction* actionCopy = myMenu.addAction(iconCopy, "Copia", parentWidget(), SLOT(slotCopy()));
    const QIcon iconPaste = QIcon::fromTheme("copy", QIcon("./images/Incolla-50.png"));
    QAction* actionPaste = myMenu.addAction(iconPaste, "Incolla", parentWidget(), SLOT(slotPaste()));
    const QIcon iconShare = QIcon::fromTheme("Information", QIcon("./images/Condividi-48.png"));
    QAction* actionShare = myMenu.addAction(iconShare, "Condividi", parentWidget(), SLOT(slotShare()));
    const QIcon iconDelete = QIcon::fromTheme("Delete", QIcon("./images/Elimina-50.png"));
    QAction* actionDelete = myMenu.addAction(iconDelete, "Elimina", parentWidget(), SLOT(slotDelete()));
    const QIcon iconInformation = QIcon::fromTheme("Information", QIcon("./images/Informazioni-50.png"));
    QAction* actionInformation = myMenu.addAction(iconInformation, "Informazioni", this, SLOT(slotShowInformation()));
    if(!f){
        actionCopy->setDisabled(true);
        actionInformation->setDisabled(true);
        actionDelete->setDisabled(true);
    }
    if(!enablePaste){
        actionPaste->setDisabled(true);
    }
    myMenu.exec(globalPosition);
}
void myUserWidget::slotShowInformation() {
    fileSystemNode* f = getTableFileSystemPointer();
    if(f){
        QMessageBox::information(this, "Informazioni sull'elemento", QString::fromStdString(f->getInformation()));
    }
    else{
        QMessageBox::critical(this, "Errore", "Attenzione, nessun elemento selezionato");
    }
}
void myUserWidget::setActionPaste(bool b) {
    enablePaste = b;
}
searchNode myUserWidget::getSearchInformation() const {
    if(completeSearch){
        byteSize minSize(0);
        byteSize maxSize(0);
        if(!minSizeLine->text().isEmpty() || !minSizeLine->text().isEmpty()){
            minSize = byteSize(minSizeLine->text().toStdString() + " " + minSizeCombo->currentText().toStdString());
            maxSize = byteSize(maxSizeLine->text().toStdString() + " " + maxSizeCombo->currentText().toStdString());
        }
        if(maxSize < minSize){
            throw feedback(0, "Attenzione, le dimensioni inserite non sono corrette");
        }
        return searchNode(searchLine->text().toStdString(), checkBoxFile->isChecked(),
                          checkBoxDirectory->isChecked(), minSize, maxSize);
    }
    else{
        return searchNode(searchLine->text().toStdString(), checkBoxFile->isChecked(),
                          checkBoxDirectory->isChecked(), 0, 0);
    }
}
void myUserWidget::setQTreeWidgetCurrentItem(fileSystemNode* f) {
    myQTreeWidgetItem* ret = findQTreeWidgetItem(currentTreeWidgetItem, f);
    if(ret){
        currentTreeWidgetItem = ret;
    }
}
myQTreeWidgetItem* myUserWidget::findQTreeWidgetItem(QTreeWidgetItem* i, fileSystemNode* f){
    myQTreeWidgetItem* item = static_cast<myQTreeWidgetItem*>(i);
    if(item->getNode() == f){
        item->setExpanded(true);
        return item;
    }
    for(int i = 0; i < item->childCount(); ++i){
        myQTreeWidgetItem* ret = findQTreeWidgetItem(item->child(i), f);
        if(ret){
            item->setExpanded(true);
            return ret;
        }
    }
    return 0;
}
myUserWidget::~myUserWidget() {}
