//
// Created by michele on 05/04/17.
//

#ifndef CLOUD_2_0_MYQTREEWIDGETITEM_H
#define CLOUD_2_0_MYQTREEWIDGETITEM_H

#include <QtWidgets/QTreeWidgetItem>
#include "model/fileSystemNode.h"

class myQTreeWidgetItem : public QTreeWidgetItem{
private:
    fileSystemNode* node;
public:
    myQTreeWidgetItem(QTreeWidgetItem*, fileSystemNode*);
    fileSystemNode* getNode() const;
    virtual ~myQTreeWidgetItem();
};


#endif //CLOUD_2_0_MYQTREEWIDGETITEM_H
