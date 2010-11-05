# -------------------------------------------------
# Project created by QtCreator 2010-08-09T11:35:58
# -------------------------------------------------
contains(QT_VERSION, ^4\\.[0-5]\\..*) { 
  error("Use at least Qt 4.6.")
}
TARGET = TrainerStudio

PROJECT_DIRECTORY = $${PWD}
message("Project $${PROJECT_DIRECTORY}")
exists(/Applications/TrainerStudio.app/FreeSWITCH/include/switch.h) { FSPATH = /Applications/TrainerStudio.app/FreeSWITCH }
exists($${PROJECT_DIRECTORY}/../freeswitch-install/include/switch.h) { FSPATH = $${PROJECT_DIRECTORY}/../freeswitch-install }
exists($${PROJECT_DIRECTORY}/../freeswitch/include/switch.h) { FSPATH = $${PROJECT_DIRECTORY}/../freeswitch }
exists($${PROJECT_DIRECTORY}/../freeswitch/src/include/switch.h) { FSPATH = $${PROJECT_DIRECTORY}/../freeswitch/src }

message("FreeSwitch: $${FSPATH}")
INCLUDEPATH += $${FSPATH}/include

win32 {
  exists($${PROJECT_DIRECTORY}/../freeswitch/w32/Library/Debug/freeswitchcore.lib) {
    LIBS = -L../freeswitch/ -L../freeswitch/w32/Library/Debug -lfreeswitchcore
  }
  else {
    LIBS = -L$${PROJECT_DIRECTORY}/../freeswitch/Win32/Debug -L../freeswitch/ -L../freeswitch/Win32/Debug -lfreeswitch
  }
}
macx {
  LIBS = -L$${FSPATH}/lib -lfreeswitch -lm
  QMAKE_CFLAGS += -isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5
  QMAKE_CXXFLAGS += -isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5
  CONFIG += x86
}

QT += network \
    webkit \
    xml \
    xmlpatterns \
    multimedia
TEMPLATE = app
SOURCES += *.cpp *.c
HEADERS += *.h
FORMS += *.ui
RESOURCES += resources.qrc
OTHER_FILES += resources/loadflash.js \
               resources/conf/freeswitch.xml \
               resources/conf/tsconf.xml
macx:ICON = resources/application.icns
win32:RC_FILE = resources.rc
