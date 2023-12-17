QT       += core gui
QT       += sql widgets
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addcategorydialog.cpp \
    addsubcategorydialog.cpp \
    addtransactiondialog.cpp \
    budget.cpp \
    database.cpp \
    deletetransactiondialog.cpp \
    linechartdialog.cpp \
    linkbutton.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    passwordresetdialog.cpp \
    registerdialog.cpp \
    transaction.cpp \
    user.cpp \
    userlogin.cpp

HEADERS += \
    accesslevel.h \
    addcategorydialog.h \
    addsubcategorydialog.h \
    addtransactiondialog.h \
    budget.h \
    database.h \
    deletetransactiondialog.h \
    linechartdialog.h \
    linkbutton.h \
    logindialog.h \
    mainwindow.h \
    passwordresetdialog.h \
    position.h \
    registerdialog.h \
    transaction.h \
    user.h \
    userlogin.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
