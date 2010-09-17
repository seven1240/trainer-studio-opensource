APP=TrainerStudio.app
RELEASE="../Trainer Studio pre-Alpha Edition"

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
cp -R ../FreeSWITCH-RELEASE $APP/FreeSWITCH

# rm -rf $RELEASE #this is dengerous
mkdir -p "$RELEASE"
cp -R $APP "$RELEASE"
cd "$RELEASE"
ln -sf /Applications .

echo Done $RELEASE
