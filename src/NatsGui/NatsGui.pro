SOURCES += \
    appmain.cpp \
    mainwindow.cpp \
    define.cpp \
    setnatsinfo.cpp \
    pubsub.cpp

HEADERS += \
    mainwindow.h \
    define.h \
    natsclient.h \
    setnatsinfo.h \
    pubsub.h

TARGET = NatsClient

RESOURCES += icon.qrc

QT += gui widgets core network

RC_FILE = logo.rc

CONFIG += C++11


DISTFILES +=

FORMS += \
    pubsub.ui
