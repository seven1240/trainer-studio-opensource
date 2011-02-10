# Overview

A VoIP Developed in Qt, using libfreeswitch for the sip part.

It is designed to work with a *server* which speaks a private JSON protocol. However, the *server* code is too specific to our business logic to be opensourced. So, we only opensource the *client* part as a thank to the FreeSWITCH community. The code is straight forward and it's easy to take out of the socket( the JSON protocol) code or you can implement your own server - you can figure out the protocol by following the code and working flows described in doc/ .

The idea is to initiate a TCP socket connection on authenticate, the remote server verify the login/pass and return back a user object (in JSON) which contains a SIP server and username/password pair, then the client can register to SIP. The advantage of using a TCP socket is that the server knows immediately if a client gone(crash/network down).

# Build

## Mac

TS depends on FreeSWITCH to be built, freeswitch must be targeted to /Applications/TrainerStudio.app/FreeSWITCH. (e.g. ./configure --prefix=/Applications/TrainerStudio.app/FreeSWITCH).


### Compile FS

You don't need FS source to compile TS, but in case you want to compile, here is the instruction:

```bash
 git clone git://git.freeswitch.org/freeswitch.git
 cd freeswitch
 ./bootstrap.sh
 #edit modules.conf, uncomment mod_port_audio
 ./configure --prefix=/Applications/TrainerStudio.app/FreeSWITCH
 make
 make install
```

**Note**: you can also install in /usr/local/freeswitch by using ./configure without --prefix, but you need to change the trainer\_studio.pro to let it can find the headers in the following step


### Compile TS

LGPL version of QT 4.7: <http://qt.nokia.com/downloads/sdk-mac-os-cpp>

Open trainer\_studio.pro in Qt creator, using shadow build.

## Windows

### It need FreeSWITCH source:

To build FS on windows, we need Visual Studio 2008 express edition. <http://www.microsoft.com/express/downloads/> VS2010 is also supported, but the last time I tried to build failed. (Aug. 2010)

clone code from git: git://git.freeswitch.org/freeswitch.git

open Freeswitch.2008.express.sln

select build solution (F7)

reference: <http://wiki.freeswitch.org/wiki/Installation_Guide#Windows>

### Build TS

Need qt 4.7. VC++ Version <http://qt.nokia.com/downloads>

freeswitch and trainer\_studio need in the same dir.

Open trainer_studio.pro in Qt Creator, using shadow build.

It is also possible to build in VS Express with the .sln. Deprecated though.

### Known issues

* ESC key make login dialog disappear, should catch it.

Moria collected some issues, sent to Jacob via email.

### Todo

* End an interaction if SIP broken.

* Webkit cache. Will need this to speed up flash loading and give better experience for trainers who has slow internet.

* Debugging tools. Like ping, traceroute etc. Make it easy to report bugs, like POST the latest logs/configs to a http server.

* compiled binaries and dlls should be on a separate file server instead of github.

* auto-upgrade feature, better installation experience. 

* Replace socket protocol to existing protocols like google's protocol buffer etc. To be discussed.

* makerelease.sh can auto gen .dmg

## Releasing

There's different ways to release

1. User install QT framework, and install TS
2. User Just install TS, with framework build in
3. Make a installer auto install QT framework if missing, or just install TS

Ideally we can get 3. work, 1. is not user friendly.  I use 2. currently.

The last TS release increases to 84M, so we definitely need to consider option 1.

We need to target to i386:

1. Support Mac 10.5
2. Support XP

There's also another reason, Flash plugin doesn't work in 64bit.(The latest Alpha plugin does work on 64bit, but it was not exist when I first write TS, pain).

### Mac

Ref: <http://doc.qt.nokia.com/4.6/deployment-mac.html>

QT already compiled in Framework. It seems we only need libfreeswitch.dylib. However, the install\_name_tools way causes core dump. So, for simplicity's sake, I just compiled FS target to /Applications/TrainerStudio.app/FreeSWITCH.

I can compile FS on 10.6.4 to i386, however, it links to 10.6 SDK. I tried to link to 10.5 SDK using -isysroot 10.5SDK but failed. So I just compiled FS on my old 10.5.2 Mac. It's in the Alpha RC2 release.

Before make a release, copy /Applications/TrainerStudio.cpp/FreeSWITCH into your shadow build dir, e.g. ../trainer\_studio-build-desktop

Then run makerelese.sh in the source tree.

### Windows

There's a inno setup .iss available. Compiled DLLs need in certain dir.

## Debugging

Win: <http://www.dependencywalker.com/> dll checker


Mac:

```bash
 otool -L xxx.dylib

 otool -l xxxx.dylib

 nm -gfj xxxxx.dylib

 file TrainerStudio

 install_name_tool
```