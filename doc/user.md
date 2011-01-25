# Trainer Studio User Doc

## Overview

Eleutian Trainer Studio (TM) is developed by Eleutian Inc. Integrated with VoIP and browser/flash Technology.

## Background

Before TS is developed, TC(short for Trainer Client) is used. The technology used in TC is 10 years old and not meet the rapid development of computer technology. So we started to develop TS.

## What changed

- Browser core changed from IE based to WebKit (which is used in Safari and Chrome) based.
- VoIP core changed to use open source FreeSWITCH.
- Mordern software look and feel with cross platform support such as Windows and Mac.

## Features and Functions

TS is designed to support multiple products of Eleutian. So, correct settings need be done on the first run.

### Server Settings

By click *Settings* on the login windows can open a settings window. And then user can choose a *Template* which related to a group of Web and VoIP servers and then click *reset* and *Save*.

*Force TCP trasport for SIP* should be used by advanced users or follow instructions from technology support.

### Audio devices

Default devices normally work, however, for users has multiple audio devices such as an USB mic/headset might want to change.

*Output Device* and Ring Device can be the same or difference, say you want to listen on a headset but want to hear a ring from you computer speaker.

*Ring File* can be the default ring tone, or choose a .wav file locally. *Only*  .wav is supported.

*Reset* can reset everything on the audio page.

*Loop Test* is designed to test the local sound devices. I.e, you speak on the mic and then can hear immediately on the headset.

Note, *Loop Test* is different that *Echo Test*. *Loop* is echoed locally on your computer(i.e. you can do that without network), and *Echo* is actually doing a call to our server and voices is echoed back from the server.

### Diagnostic

*Enable Debug* is designed for advanced users.

*Clear Cache* - Clear the browser cache, i.e. downloaded html and swf files. It only affect to the internal embedded browser and has no effect to other browsers such as IE and FireFox.

*FS Console* - For advanced users only.

*Reveal log file in folder* - Can open a directory to browse the log file on user's computer. Sometimes technology support people might require a log to find problems. This button is designed to find logs easily.

### The main window

You can only click *Start Working* when you see *SIP: REGED* on the status bar(the very bottom). Sometimes you might lost VoIP(SIP) connection to server, TS will try to re-register shortly but a *register* button will also appear so you can retry the *register* action immediately. Clickingthe *register* button does no harm to TS and the servers, but you won't get any benefit by clicking it so frequently if servers or network has problem.

You will be force *Paused* when the server knows you have problem to get a call.

### The browser window

Also can call the *Interaction window* or the *Flash window*. It is an embedded browser such as IE and FireFox which can load flash and webpages during an interaction.

*Timer* - Time for interaction or review. It is counting up other than count down in the old TC.
*Mute* - Mute or UnMute your mic so the other party(student/learner) cannot hear you.
*Reload* - Reload the current page. It is safe to reload if something went wrong, like the *bubbles* problem in TC.
*Copy ID* - Copy the interaction ID to your clipboard so you can paste somewhere.
*Reconnect* - Reconnect hangup the student side and re-call them, helpful if sound was bad or getting worse. Note, you can only reconnect a few times on the current settings, normally 3 or 4.
*Hangup* - Hangup the call and go to review.
*Finish* - on the review screen, the *Finish* button is actually on the flash page but not on the window. Be sure to click *Finish* instead of the X icon on the window.

### The call window

Click *call* on the main window will open a call window. It is like a softphone but can only call limited numbers. There are shortcuts buttons such as *Conf 20* (conference room 20) or *Echo Test*. Note, you still need to click the *Call* button after those shortcuts.

*Do not open this window* when you plan to do interaction.

### For advanced users

This section is for technology support or supervisors.

By Enabling debug you can see two more buttons, one is called *Flash* on the main window. After click it it can open a Flash window and an extra *Test* button also available to try load different pages.

	Settings -> Diagnostic -> Enable Debug 

1st click - Open the Adobe Flash site, so you knows your version of flash plugin.
2nd click - Open a sample Interaction screen.
3rd click - Open a sample review screen.
4th? - go to 1st.

LOGs.

Click *reveal log files in folder* on the *Diagnostic* window will guide you to the log folder. 

freeswitch.log - VoIP log
trainer\_studio.log - TS log(the current)
trainer\_studio\_xxxxx.log - TS log, old ones, named by closing date.

Generally technical support would like to see the first 2 logs, and sometimes trainer\_studio\_xxxx.log(find the most recent one) is also needed.


