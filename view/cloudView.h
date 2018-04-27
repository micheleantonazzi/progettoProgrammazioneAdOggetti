//
// Created by michele on 24/03/17.
//

#ifndef CLOUD_2_0_CLOUDVIEW_H
#define CLOUD_2_0_CLOUDVIEW_H


#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QXmlStreamWriter>
#include <QFormLayout>
#include "myLoginWidget.h"
#include "myUserWidget.h"
#include "model/cloudModel.h"
#include "model/feedback.h"
#include "QMenu"
#include "QMenuBar"
#include "QAction"
#include "myChangeInformationDialog.h"

class cloudView : public QMainWindow {
Q_OBJECT
private:
    cloudModel *model;
    myLoginWidget *widgetLogin;
    myUserWidget *userWidget;
    QToolBar* toolBar;
    QAction* actionAddDir;
    QAction* actionAddFile;
    QAction* actionLevelUp;
    QAction* actionDelete;
    QAction* actionCopy;
    QAction* actionPaste;
    QAction* actionShare;
    QAction* actionRename;
    fileSystemNode* moveNode;
    QDialog* searchDialog;
    QTableWidget* searchTable;
    myChangeInformationDialog* changeInformationDialog;
    void writeTreeXml(QXmlStreamWriter&, fileSystemNode*) const;
    fileSystemNode* createTree(QXmlStreamReader&) const;
    void setWidgetLogin();
    void setWidgetUser();
    void createToolBar();
signals:
public slots:
    void slotBackup();
    void slotLogin();
    void slotAddUser();
    void slotAddDirectory();
    void slotAddFile();
    void openDirectory(directoryNode*);
    void slotLevelUp();
    void slotEnableActions();
    void slotDelete();
    void slotCopy();
    void slotPaste();
    void slotShare();
    void slotRename();
    void slotSearch();
    void slotOpenSearchNode(int, int);
    void slotLogout();
    void slotSettings();
    void slotConfirmSettings();
    void slotUpgradeAccount();
    void slotDeleteAccount();
    void slotAccountInformation();
public:
    cloudView(cloudModel*);
    void restore();
    void disableActions();
    void backup() const;
    void disableAdminActions() const;
    ~cloudView();
};


#endif //CLOUD_2_0_CLOUDVIEW_H
