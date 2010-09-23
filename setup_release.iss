; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{40E91EA5-4CC0-4C23-8262-864ABF160258}
AppName=Idapted Trainer Studio
AppVersion=0.0.1
;AppVerName=Idapted Trainer Studio 0.0.1
AppPublisher=Idapted Inc.
AppPublisherURL=http://www.idapted.com
AppSupportURL=http://www.idapted.com/trainer
AppUpdatesURL=http://www.idapted.com/trainer
DefaultDirName={pf}\Idapted\Idapted Trainer Studio
DefaultGroupName=Idapted Trainer Studio
AllowNoIcons=yes
LicenseFile=license.txt
OutputDir=.
OutputBaseFilename=trainer-studio-setup-beta-release
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "D:\trainer_studio\trainer_studio\Release\TrainerStudio.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\FreeSwitch.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\fs_cli.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\dwmapi.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\FreeSwitch.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\libapr.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\libaprutil.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\libspandsp.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\libteletone.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\msvcr90.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\msvcr90d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\Microsoft.VC90.DebugCRT.manifest"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\pthreadVC2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\phonon4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\QtWebKit4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\QtXml4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\QtXmlPatterns4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Release\dlls\NPSWF32.dll"; DestDir: "{sys}\Macromed\Flash";
Source: "D:\trainer_studio\trainer_studio\Release\mod\*"; DestDir: "{app}/mod"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Idapted Trainer Studio"; Filename: "{app}\TrainerStudio.exe"
Name: "{group}\{cm:UninstallProgram,Idapted Trainer Studio}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Idapted Trainer Studio"; Filename: "{app}\TrainerStudio.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\TrainerStudio.exe"; Description: "{cm:LaunchProgram,Idapted Trainer Studio}"; Flags: nowait postinstall skipifsilent

