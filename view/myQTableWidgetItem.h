//
// Created by michele on 26/04/17.
//

#ifndef CLOUD_2_0_MYQTABLEWIDETITEM_H
#define CLOUD_2_0_MYQTABLEWIDETITEM_H


#include <QtWidgets/QTableWidgetItem>
#include "model/fileSystemNode.h"

class myQTableWidgetItem : public QTableWidgetItem{
private:
    fileSystemNode* node;
public:
    myQTableWidgetItem(fileSystemNode*);
    fileSystemNode* getNodePointer() const;
    virtual ~myQTableWidgetItem();
};


#endif //CLOUD_2_0_MYQTABLEWIDETITEM_H
