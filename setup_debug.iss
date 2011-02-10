; Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
; Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
; 
; Version: Apache License 2.0
; 
; The content of this file is licensed under the Apache License, Version 2.0. (the "License").
; You may not use this file except in compliance with
; the License. You may obtain a copy of the License at
; http://www.apache.org/licenses/
; 
; Software distributed under the License is distributed on an "AS IS" basis,
; WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
; for the specific language governing rights and limitations under the
; License.
; 
; The Original Code is Eleutian Trainer Studio Application Opensource Edition.
; 
; The Initial Developer of the Original Code is
; Sonic Guo <sonic@eleutian.com>
; Portions created by the Initial Developer are Copyright (C)
; the Initial Developer. All Rights Reserved.
; 
; Contributor(s):
; 

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{40E91EA5-4CC0-4C23-8262-864ABF160258}
AppName=Idapted Trainer Studio
AppVersion=0.0.1
;AppVerName=Idapted Trainer Studio 0.0.1
AppPublisher=Idapted Inc.
AppPublisherURL=http://www.eleutian.com
AppSupportURL=http://www.eleutian.com/trainer
AppUpdatesURL=http://www.eleutian.com/trainer
DefaultDirName={pf}\Eleutian\Eleutian Trainer Studio
DefaultGroupName=Idapted Trainer Studio
AllowNoIcons=yes
LicenseFile=license.txt
OutputDir=.
OutputBaseFilename=trainer-studio-setup-beta-debug
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "D:\trainer_studio\trainer_studio\Debug\TrainerStudio.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\dwmapi.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\FreeSwitch.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\libapr.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\libaprutil.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\libspandsp.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\libteletone.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\msvcr90.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\pthreadVC2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\qjson0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\phonond4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\QtCored4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\QtGuid4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\QtNetworkd4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\QtWebKitd4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\QtXmld4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\QtXmlPatternsd4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\trainer_studio\trainer_studio\Debug\dlls\NPSWF32.dll"; DestDir: "{sys}\Macromed\Flash";
Source: "D:\trainer_studio\trainer_studio\Debug\mod\*"; DestDir: "{app}/mod"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Idapted Trainer Studio"; Filename: "{app}\TrainerStudio.exe"
Name: "{group}\{cm:UninstallProgram,Idapted Trainer Studio}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Idapted Trainer Studio"; Filename: "{app}\TrainerStudio.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\TrainerStudio.exe"; Description: "{cm:LaunchProgram,Idapted Trainer Studio}"; Flags: nowait postinstall skipifsilent

