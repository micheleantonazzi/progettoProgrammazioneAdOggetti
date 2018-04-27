#-------------------------------------------------
#
# Project created by CLion 2017-05-02T17:25:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cloud_2.0
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
	model/byteSize.cpp \
	model/feedback.cpp \
	model/fileSystemNode.cpp \
	model/fileNode.cpp \
	model/fileNodeShared.cpp \
	model/directoryNode.cpp \
	model/directoryNodeShared.cpp \
	view/labelClicked.cpp \
	model/user.cpp \
	model/rootNode.cpp \
	model/standardUser.cpp \
	model/premiumUser.cpp \
	model/admin.cpp \
	model/cloudModel.cpp \
	view/cloudView.cpp \
	view/myLoginWidget.cpp \
	view/myUserWidget.cpp \
	view/myQTreeWidgetItem.cpp \
	view/myQTableWidgetItem.cpp \
	model/searchNode.cpp \
	view/myChangeInformationDialog.cpp \
	model/fileSystemNodeShared.cpp \

HEADERS += view/cloudView.h \ model/byteSize.h \
	model/feedback.h \
	model/fileSystemNode.h \
	model/fileNode.h \
	model/fileNodeShared.h \
	model/directoryNode.h \
	model/directoryNodeShared.h \
	view/labelClicked.h \
	model/user.h \
	model/rootNode.h \
	model/standardUser.h \
	model/premiumUser.h \
	model/admin.h \
	model/cloudModel.h \
	view/myLoginWidget.h \
	view/myUserWidget.h \
	view/myQTreeWidgetItem.h \
	view/myQTableWidgetItem.h \
	model/searchNode.h \
	view/myChangeInformationDialog.h \
	model/fileSystemNodeShared.h \
