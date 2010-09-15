#/bin/sh

install_name_tool -change /opt/Frameworks/it-must-be-a-very-long-path-to-make-sure-it-has-enough-room-to-change-the-install-name/Qt/lib/QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore libqjson.dylib
