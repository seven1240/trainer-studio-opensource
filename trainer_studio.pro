# -------------------------------------------------
# Project created by QtCreator 2010-08-09T11:35:58
# -------------------------------------------------
contains(QT_VERSION, ^4\\.[0-5]\\..*) { 
    message("Cannot build with Qt version $$QT_VERSION.")
    error("Use at least Qt 4.6.")
}
TARGET = TrainerStudio
#INCLUDEPATH = $${FSPATH}/src/include \
#    $${FSPATH}/libs/apr/include \
#    $${FSPATH}/libs/libteletone/src \
#    libs/qjson/src

PROJECT_DIRECTORY = $$system(pwd)
message("Project $${PROJECT_DIRECTORY}")
exists(/Applications/TrainerStudio.app/FreeSWITCH/include/switch.h) { FSPATH = /Applications/TrainerStudio.app/FreeSWITCH }
exists($${PROJECT_DIRECTORY}/../freeswitch-install/include/switch.h) { FSPATH = $${PROJECT_DIRECTORY}/../freeswitch-install }
exists($${PROJECT_DIRECTORY}/../freeswitch/include/switch.h) { FSPATH = $${PROJECT_DIRECTORY}/../freeswitch }
message("Using $${FSPATH}")

INCLUDEPATH = $${FSPATH}/include
macx:LIBS = -L$${FSPATH}/lib -lfreeswitch -lm
win32:LIBS = -L../freeswitch/ -L../freeswitch/w32/Library/Debug -lfreeswitchcore

#!win32:!macx {
#    # This is here to comply with the default freeswitch installation
#    QMAKE_LFLAGS += -Wl,-rpath,/usr/local/freeswitch/lib
#    LIBS += -lcrypt \
#        -lrt
#}

macx:QMAKE_CFLAGS += -isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5
macx:QMAKE_CXXFLAGS += -isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5

CONFIG += x86 # 32bit binary
QT += network \
    webkit \
    xml \
    xmlpatterns \
    multimedia
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
    IncomingCallDialog.cpp \
    SettingsDialog.cpp \
    EchoTestDialog.cpp \
    cjson.c \
    qJSON.cpp \
    Utils.cpp
HEADERS += mainwindow.h \
    flashdialog.h \
    fshost.h \
    trainer_studio.h \
    channel.h \
    call.h \
    logindialog.h \
    TCPClient.h \
    isettings.h \
    IncomingCallDialog.h \
    SettingsDialog.h \
    EchoTestDialog.h \
    cjson.h \
    qJSON.h \
    Utils.h
FORMS += mainwindow.ui \
    flashdialog.ui \
    logindialog.ui \
    IncomingCallDialog.ui \
    SettingsDialog.ui \
    EchoTestDialog.ui
RESOURCES += resources.qrc
OTHER_FILES += resources/loadflash.js \
    resources/conf/freeswitch.xml \
    resources/conf/tsconf.xml
macx:ICON = resources/application.icns
win32:RC_FILE = resources.rc
