# Overview

Developed in Qt, using libfreeswitch for the sip part.

# Build

## Mac

TS depends on FreeSWITCH to be built, just download and install TS in /Applicaions:

CN server: <http://www.eqenglish.com/assets/trainer_files/Trainer-Studio-Alpha-Edition-RC2.dmg>
US server: <http://www.idapted.com/assets/trainer_files/Trainer-Studio-Alpha-Edition-RC2.dmg>

LGPL version of QT 4.7: <http://qt.nokia.com/downloads/sdk-mac-os-cpp>

Open trainer\_studio.pro in Qt creator, using shadow build.

Just upgrade to QT 4.7, so the mkrelease.sh might not work.

## Windows

### It need FreeSWITCH source:

To build FS on windows, we need Visual Studio 2008 express edition. <http://www.microsoft.com/express/downloads/> VS2010 is also supported, but the last time I tried to build failed. (Aug. 2010)

clone code from git: git://git.freeswitch.org/freeswitch.git

open Freeswitch.2008.express.sln

select build solution (F7)

reference: <http://wiki.freeswitch.org/wiki/Installation_Guide#Windows>

### TS

Need qt 4.7. VC++ Version <http://qt.nokia.com/downloads>

freeswitch and trainer\_studio need in the same dir.

Open trainer_studio.pro in Qt Creator, using shadow build.

It is also possible to build in VS Express with the .sln. Deprecated though.

## Debugging

Win: <http://www.dependencywalker.com/> dll checker


Mac:

```
 otool -L xxx.dylib
 otool -l xxxx.dylib
 nm -gfj xxxxx.dylib
 file TrainerStudio
 install_name_tool
```