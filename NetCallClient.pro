#-------------------------------------------------
#
# Project created by QtCreator 2019-01-24T21:17:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetCallClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    callingdialognumberpanel.cpp \
    cmultisipmanager.cpp \
    dialognumberpanel.cpp \
        main.cpp \
    sipaccountinfo.cpp \
    switchpanelwidget.cpp \
    testmaindialog.cpp \
        widget.cpp \
    zconfigsettings.cpp \
    zsettingpage.cpp \
    dialogsnumber.cpp \
    titlebar.cpp \
    zmainwidget.cpp

HEADERS += \
    callingdialognumberpanel.h \
    cmultisipmanager.h \
    dialognumberpanel.h \
    publicstruct.h \
    sipaccountinfo.h \
    switchpanelwidget.h \
    testmaindialog.h \
        widget.h \
    zconfigsettings.h \
    zsettingpage.h \
    dialogsnumber.h \
    titlebar.h \
    zmainwidget.h

FORMS += \
    callingdialognumberpanel.ui \
    dialognumberpanel.ui \
    sipaccountinfo.ui \
    switchpanelwidget.ui \
    testmaindialog.ui \
        widget.ui \
    zsettingpage.ui \
    dialogsnumber.ui

#sip的相关库
INCLUDEPATH += $$PWD/sipLib/pjsip/include \
                $$PWD/sipLib/pjnath/include \
                $$PWD/sipLib/pjmedia/include \
                $$PWD/sipLib/pjlib-util/include \
                $$PWD/sipLib/pjlib/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/sipLib/lib -llibpjproject-i386-Win32-vc14-Debug-Dynamic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/sipLib/lib -llibpjproject-i386-Win32-vc14-Release-Dynamic

LIBS += -lWs2_32 -lOle32 -lgdi32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    README.md
