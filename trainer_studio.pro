# -------------------------------------------------
# Project created by QtCreator 2010-08-09T11:35:58
# -------------------------------------------------
contains(QT_VERSION, ^4\.[0-5]\..*) { 
    message("Cannot build with Qt version $$QT_VERSION.")
    error("Use at least Qt 4.6.")
}

TARGET = TrainerStudio
FSPATH = ../freeswitch

INCLUDEPATH = $${FSPATH}/src/include \
    $${FSPATH}/libs/apr/include \
    $${FSPATH}/libs/libteletone/src \
    libs/qjson/src
!win32 {
    LIBS = -L/Applications/FreeSWITCH/lib \
        -lfreeswitch \
        -lm \
        -Llibs/qjson/lib \
        -lqjson
}

win32 {
    LIBS = -L../freeswitch/ \
        -L../freeswitch/w32/Library/Debug \
        -lfreeswitchcore \
        -L../trainer_studio/libs/qjson/lib \
        -lqjson0
}

!win32:!macx { 
    # This is here to comply with the default freeswitch installation
    QMAKE_LFLAGS += -Wl,-rpath,/usr/local/freeswitch/lib
    LIBS += -lcrypt \
        -lrt
}
CONFIG += x86 # 32bit binary
QT += network \
    webkit \
    xml \
    xmlpatterns \
    multimedia \
    testlib

TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    flashdialog.cpp \
    fshost.cpp \
    channel.cpp \
    call.cpp \
    logindialog.cpp \
    TCPClient.cpp \
    isettings.cpp \
    IncomingCallDialog.cpp
HEADERS += mainwindow.h \
    flashdialog.h \
    fshost.h \
    trainer_studio.h \
    channel.h \
    call.h \
    logindialog.h \
    TCPClient.h \
    isettings.h \
    IncomingCallDialog.h
FORMS += mainwindow.ui \
    flashdialog.ui \
    logindialog.ui \
    IncomingCallDialog.ui
RESOURCES += resources.qrc
OTHER_FILES += conf/freeswitch.xml \
    resources/loadflash.js
