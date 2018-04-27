//
// Created by michele on 08/03/17.
//

#ifndef CLOUD_2_0_LABELCLICKED_H
#define CLOUD_2_0_LABELCLICKED_H

#include <QLabel>

class labelClicked : public QLabel{
Q_OBJECT
signals:
    void clicked();
protected:
    void mousePressEvent ( QMouseEvent * event ) ;
public:
    labelClicked(QWidget* = 0);
};

#endif //CLOUD_2_0_LABELCLICKED_H
