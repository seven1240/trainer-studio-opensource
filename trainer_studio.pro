# -------------------------------------------------
# Project created by QtCreator 2010-08-09T11:35:58
# -------------------------------------------------

contains(QT_VERSION, ^4\.[0-5]\..*) {
    message("Cannot build with Qt version $$QT_VERSION.")
    error("Use at least Qt 4.6.")
}

TARGET = trainer_studio
macx:TARGET = TrainerStudio

FSPATH = ../../freeswitch/freeswitch
INCLUDEPATH = $${FSPATH}/src/include \
    $${FSPATH}libs/apr/include \
    $${FSPATH}/libs/libteletone/src
LIBS = -L$${FSPATH}/.libs \
    -lfreeswitch \
    -lm
!win32:!macx {
    # This is here to comply with the default freeswitch installation
    QMAKE_LFLAGS += -Wl,-rpath,/usr/local/freeswitch/lib
    LIBS += -lcrypt \
        -lrt
}

# xCONFIG += x86   # 32bit binary

QT += network \
    webkit \
    xml \
    xmlpatterns \
    multimedia \
    testlib
TARGET = trainer_studio
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    flashdialog.cpp \
    fshost.cpp \
    channel.cpp \
    call.cpp
HEADERS += mainwindow.h \
    flashdialog.h \
    fshost.h \
    trainer_studio.h \
    channel.h \
    call.h
FORMS += mainwindow.ui \
    flashdialog.ui
