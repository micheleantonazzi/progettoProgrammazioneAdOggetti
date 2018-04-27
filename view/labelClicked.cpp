//
// Created by michele on 08/03/17.
//

#include "labelClicked.h"
#include <string>
labelClicked::labelClicked(QWidget *parent) : QLabel(parent) {}
void labelClicked::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}