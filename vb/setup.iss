; Script for Trainer Client Setup

[Setup]
AppName=Idapted Trainer Client
AppVerName=Idapted Trainer Client 3.0.2
AppPublisher=Idapted Inc.
AppPublisherURL=http://www.idapted.com
AppSupportURL=http://trainer.idapted.com
AppUpdatesURL=http://trainer.idapted.com
DefaultDirName={pf}\Idapted
DisableProgramGroupPage=yes
DefaultGroupName=Idapted
OutputBaseFilename=trainer-client-setup-beta
Compression=lzma
SolidCompression=yes
LicenseFile=license.txt
OutputDir=.
[Languages]
Name: "english";MessagesFile:"compiler:Default.isl"

[Tasks]

[Files]
Source: "libcurl.dll"; DestDir: "{sys}"; Flags: sharedfile
Source: "libeay32.dll"; DestDir: "{sys}"; Flags: sharedfile
Source: "ssleay32.dll"; DestDir: "{sys}"; Flags: sharedfile
Source: "zlib1.dll"; DestDir: "{sys}"; Flags: sharedfile
Source: "cSystemInfoAX.ocx"; DestDir: "{sys}"; Flags: sharedfile regserver
Source: "MSWINSCK.OCX"; DestDir: "{sys}"; Flags: sharedfile regserver

Source: "RYVE.dll"; DestDir: "{sys}"; CopyMode: alwaysoverwrite; Flags: ignoreversion regserver sharedfile
Source: "comctl32.ocx"; DestDir: "{sys}"; Flags: regserver sharedfile
;Source: "msvbvm50.dll"; DestDir: "{sys}"; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall sharedfile regserver
Source: "msvbvm60.dll"; DestDir: "{sys}"; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall sharedfile regserver
Source: "MSINET.ocx"; DestDir: "{sys}"; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall sharedfile regserver
Source: "IdaptedTrainerClient.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "ring.wav"; DestDir: "{app}"; Flags: ignoreversion
;Source: "sounds\*"; DestDir: "{app}\sounds"; Flags: ignoreversion
[Icons]
Name: "{group}\Trainer Client"; Filename: "{app}\IdaptedTrainerClient.exe"
[Run]
Filename: "{app}\IdaptedTrainerClient.exe"; Description: "{cm:LaunchProgram,IdaptedTrainerClient}"; Flags: nowait postinstall skipifsilent
