#!/bin/bash

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


SHADOW_DIR=../trainer_studio-build-desktop
APP=TrainerStudio.app
APP_DIR=$SHADOW_DIR/$APP
RELEASE="$SHADOW_DIR/Trainer-Studio-Alpha-Edition-RC3"

echo "APP: $APP"
echo "APP_DIR: $APP_DIR"
echo "RELEASE: $RELEASE"

echo "Copying QT Framework ..."
macdeployqt $APP_DIR

echo "Strping down QT Framework to i386"
pushd $APP_DIR/Contents/Frameworks
for f in phonon QtCore QtDBus QtGui QtMultimedia QtNetwork QtOpenGL QtWebKit QtXml QtXmlPatterns
do
	find . -name $f -exec ditto --rsrc --arch i386 {} {}-i386 \;
	find . -name $f -exec mv {}-i386 {} \;
done
popd


#install_name_tools -change on libfreeswitch causes throw error with no reason
# Assertion failed: (BLOCK != ((void *)0)), function switch_event_bind_removable, file src/switch_event.c, line 1392.
#change back
install_name_tool -change @executable_path/../Frameworks/libfreeswitch.1.dylib \
	/Applications/TrainerStudio.app/FreeSWITCH/lib/libfreeswitch.1.dylib \
	$APP_DIR/Contents/MacOS/TrainerStudio
	
#Let's don't use this one
rm -f $APP_DIR/Contents/Frameworks/libfreeswitch.1.dylib

cp -R resources/* $APP_DIR/Contents/Resources/
find $APP_DIR/Contents/Resources/ -name .svn -exec rm -rf {} \;

cp INSTALL.txt "$RELEASE/"

echo "Copying Flash plugin..."
mkdir -p "$RELEASE/FlashPlugin"
cp "/Library/Internet Plug-Ins/flashplayer.xpt" "$RELEASE/FlashPlugin/"
cp -R "/Library/Internet Plug-Ins/Flash Player.plugin" "$RELEASE/FlashPlugin/"

echo "Linking /Applications"
# rm -rf $RELEASE #this is dengerous
cp -R $APP_DIR "$RELEASE"
ln -sf /Applications "$RELEASE"


FS="$RELEASE/$APP/FreeSWITCH"
FSR="$SHADOW_DIR/FreeSWITCH"

echo "FS: $FS"
echo "FSR: $FSR"

mkdir -p "$FS/lib" "$FS/mod" "$FS/bin"

echo "Copying FreeSWITCH headers..."
cp -Rv $FSR/include "$FS/"

echo "Copying FreeSWITCH modules..."

for mod in \
mod_logfile \
mod_cdr_csv \
mod_sofia \
mod_loopback \
mod_commands \
mod_dptools \
mod_dialplan_xml \
mod_spandsp \
mod_ilbc \
mod_speex \
mod_celt \
mod_silk \
mod_siren \
mod_sndfile \
mod_tone_stream \
mod_local_stream \
mod_portaudio \
mod_event_socket; do
cp $FSR/mod/$mod.so "$FS/mod/"
cp $FSR/bin/fs_cli "$FS/bin/"
cp $FSR/bin/freeswitch "$FS/bin/"
cp -v $FSR/lib/libfreeswitch*.dylib "$FS/lib/"
done

cd "$FS/lib"
ln -sf libfreeswitch.1.dylib libfreeswitch.dylib
ln -sf libfreeswitch.1.dylib libfreeswitch.1.0.0.dylib

echo Done $RELEASE
