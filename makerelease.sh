APP=TrainerStudio.app
RELEASE="release/Trainer Studio pre-Alpha Edition"

macdeployqt $APP
mkdir -p $APP/Contents/Frameworks
cp libs/qjson/lib/libqjson.dylib $APP/Contents/Frameworks/

#change back, or it will throw with no reason
# Assertion failed: (BLOCK != ((void *)0)), function switch_event_bind_removable, file src/switch_event.c, line 1392.

install_name_tool -change @executable_path/../Frameworks/libfreeswitch.1.dylib \
	/Applications/TrainerStudio.app/FreeSWITCH/lib/libfreeswitch.1.dylib \
	$APP/Contents/MacOS/TrainerStudio

ln -sf /Applications $APP/Applications
cp -R resources/{conf,images,loadflash.js} TrainerStudio.app/Contents/Resources/
cp -R ../FreeSWITCH-RELEASE TrainerStudio.app/FreeSWITCH

mkdir -p "$RELEASE"
cp -R $APP "$RELEASE"
cd release
ln -sf /Applications .
