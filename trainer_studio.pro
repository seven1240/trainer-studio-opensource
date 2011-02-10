# -------------------------------------------------
# Project created by QtCreator 2010-08-09T11:35:58
# -------------------------------------------------
#
# Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
# Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
# 
# Version: Apache License 2.0
# 
# The content of this file is licensed under the Apache License, Version 2.0. (the "License").
# You may not use this file except in compliance with
# the License. You may obtain a copy of the License at
# http://www.apache.org/licenses/
# 
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.
# 
# The Original Code is Eleutian Trainer Studio Application Opensource Edition.
# 
# The Initial Developer of the Original Code is
# Seven Du <seven@eleutian.com>
# Portions created by the Initial Developer are Copyright (C)
# the Initial Developer. All Rights Reserved.
# 
# Contributor(s):
#
# Jadob Lewallen <jacob@eleutian.com>
#

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
SOURCES += $${PROJECT_DIRECTORY}/lib/qtsingleapplication-2.6.1/*.cpp
HEADERS += $${PROJECT_DIRECTORY}/lib/qtsingleapplication-2.6.1/*.h
FORMS += *.ui
RESOURCES += resources.qrc
OTHER_FILES += resources/loadflash.js \
               resources/conf/freeswitch.xml \
               resources/conf/tsconf.xml
macx:ICON = resources/application.icns
win32:RC_FILE = resources.rc

