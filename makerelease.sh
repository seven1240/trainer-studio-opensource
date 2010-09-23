APP=TrainerStudio.app
RELEASE="../Trainer Studio Alpha Edition RC2"

macdeployqt $APP
mkdir -p $APP/Contents/Frameworks

#change back, or it will throw with no reason
# Assertion failed: (BLOCK != ((void *)0)), function switch_event_bind_removable, file src/switch_event.c, line 1392.

install_name_tool -change @executable_path/../Frameworks/libfreeswitch.1.dylib \
	/Applications/TrainerStudio.app/FreeSWITCH/lib/libfreeswitch.1.dylib \
	$APP/Contents/MacOS/TrainerStudio
rm -f $APP/Contents/Frameworks/libfreeswitch.1.dylib

cp -R resources/* $APP/Contents/Resources/
find $APP/Contents/Resources/ -name .svn -exec rm -rf {} \;

mkdir -p "$RELEASE/FlashPlugin"
cp "/Library/Internet Plug-Ins/flashplayer.xpt" "$RELEASE/FlashPlugin/"
cp -R "/Library/Internet Plug-Ins/Flash Player.plugin" "$RELEASE/FlashPlugin/"

# rm -rf $RELEASE #this is dengerous
cp -R $APP "$RELEASE"
cd "$RELEASE"
ln -sf /Applications .

FS="$RELEASE/$APP/FreeSWITCH"
FSR=../FreeSWITCH-RELEASE-10.5
mkdir -p "$FS/lib" "$FS/mod" "$FS/bin"
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
cp $FSR/lib/libfreeswitch.1.dylib "$FS/lib/"
done

echo Done $RELEASE
