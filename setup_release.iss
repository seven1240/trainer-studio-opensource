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
AppId={{AF5513A1-41A9-4985-ABFB-76B77831A9EB}
AppName=Eleutian Trainer Studio
AppVersion=3.0.3
AppVerName=Eleutian Trainer Studio 3.0.3
AppPublisher=Eleutian Inc.
AppPublisherURL=http://www.eleutian.com
AppSupportURL=http://www.eleutian.com/trainer
AppUpdatesURL=http://www.eleutian.com/trainer
DefaultDirName={pf}\Eleutian\Eleutian Trainer Studio
DefaultGroupName=Eleutian Trainer Studio
AllowNoIcons=yes
LicenseFile=license.txt
OutputDir=.
OutputBaseFilename=trainer-studio-setup-release
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "Release\TrainerStudio.exe"; DestDir: "{app}"; Flags: ignoreversion
; Source: "Release\FreeSwitch.exe"; DestDir: "{app}"; Flags: ignoreversion
; Source: "Release\fs_cli.exe"; DestDir: "{app}"; Flags: ignoreversion
; Source: "Release\dlls\dwmapi.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\FreeSwitch.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\libapr.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\libaprutil.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\libspandsp.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\libteletone.dll"; DestDir: "{app}"; Flags: ignoreversion
; Source: "Release\dlls\msvcr90.dll"; DestDir: "{app}"; Flags: ignoreversion
; Source: "Release\dlls\msvcr90d.dll"; DestDir: "{app}"; Flags: ignoreversion
; Source: "Release\dlls\Microsoft.VC90.DebugCRT.manifest"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\pthreadVC2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\phonon4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\QtWebKit4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\QtXml4.dll"; DestDir: "{app}"; Flags: ignoreversion
; Source: "Release\dlls\QtXmlPatterns4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\dlls\NPSWF32.dll"; DestDir: "{sys}\Macromed\Flash";
Source: "Release\dlls\pthreadVC2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\mod\*"; DestDir: "{app}/mod"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Eleutian Trainer Studio"; Filename: "{app}\TrainerStudio.exe"
Name: "{group}\{cm:UninstallProgram,Eleutian Trainer Studio}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Eleutian Trainer Studio"; Filename: "{app}\TrainerStudio.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\TrainerStudio.exe"; Description: "{cm:LaunchProgram,Eleutian Trainer Studio}"; Flags: nowait postinstall skipifsilent

