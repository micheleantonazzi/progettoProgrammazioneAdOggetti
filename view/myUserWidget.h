//
// Created by michele on 03/04/17.
//

#ifndef CLOUD_2_0_MYUSERWIDGET_H
#define CLOUD_2_0_MYUSERWIDGET_H


#include <QWidget>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include "model/fileSystemNode.h"
#include "model/rootNode.h"
#include "myQTreeWidgetItem.h"
#include "model/user.h"
#include "QLineEdit"

class myUserWidget : public QWidget{
Q_OBJECT
private:
    QTreeWidget *treeWidget;
    QTableWidget *tableWidget;
    QTreeWidgetItem* currentTreeWidgetItem;
    QLineEdit* searchLine;
    QCheckBox* checkBoxFile;
    QCheckBox* checkBoxDirectory;
    QLineEdit* minSizeLine;
    QLineEdit* maxSizeLine;
    QComboBox* minSizeCombo;
    QComboBox* maxSizeCombo;
    QPushButton* searchButton;
    bool enablePaste;
    bool completeSearch;
    void createTree(fileSystemNode*, QTreeWidgetItem*) const;
    static myQTreeWidgetItem* findQTreeWidgetItem(QTreeWidgetItem*, fileSystemNode*);
signals:
    void openDirectory(directoryNode*);
public slots:
    void slotTableOpenDirectory(const QModelIndex&);
    void slotTreeOpenDirectory(QTreeWidgetItem*, int);
    void showContextMenu(const QPoint&);
    void slotShowInformation();
    void slotEnableSearchButton();
public:
    myUserWidget(QWidget*, bool);
    void createTreeWidget(directoryNode*);
    void createUserTreeWidget(directoryNode*);
    void QTreeWidgetItemLevelUp();
    void createTableWidget(list <fileSystemNode*>) const;
    void createUserTableWidget(list<user *>) const;
    void addQTreeWidgetItem(fileSystemNode*, int) const;
    void removeQTreeWidgetItem(fileSystemNode*) const;
    fileSystemNode* getTableFileSystemPointer() const;
    void setActionPaste(bool);
    searchNode getSearchInformation() const;
    void setQTreeWidgetCurrentItem(fileSystemNode*);
    ~myUserWidget();
};


#endif //CLOUD_2_0_MYUSERWIDGET_H
