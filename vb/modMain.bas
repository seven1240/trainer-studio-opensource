Attribute VB_Name = "modMain"
Option Explicit
 
Public Declare Function BringWindowToTop Lib "user32" (ByVal _
         hWnd As Long) As Long
         
Public AudioInDev As Long, AudioOutDev As Long
Public PromptScreenWidth As Long
Public PromptScreenHeight As Long
Public PromptScreenTop As Long
Public PromptScreenLeft As Long
Public PromptScreenState As Long
Public FlashUniqueId As String
Public UserType As String
Public Unloading As Boolean
Public AlreadyCompletedEchoTest As Boolean
  
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Public Const SW_SHOW = 5       ' Displays Window in its current size
                                      ' and position
Public Const SW_SHOWNORMAL = 1 ' Restores Window if Minimized or

                                      ' Maximized
    

Public Declare Function ShellExecute Lib "SHELL32.DLL" Alias _
         "ShellExecuteA" (ByVal hWnd As Long, ByVal lpOperation As _
         String, ByVal lpFile As String, ByVal lpParameters As String, _
         ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long



Public Enum PRIORITY_CLASS
   NORMAL_PRIORITY_CLASS = &H20
   IDLE_PRIORITY_CLASS = &H40
   HIGH_PRIORITY_CLASS = &H80
   REALTIME_PRIORITY_CLASS = &H100
End Enum
Private Declare Function GetLastError Lib "kernel32.dll" () As Long
Private Const PROCESS_DUP_HANDLE = &H40
Private Declare Function OpenProcess Lib "kernel32" _
   (ByVal dwDesiredAccess As Long, _
    ByVal bInheritHandle As Long, _
    ByVal dwProcessId As Long) As Long
    
Private Declare Function CloseHandle Lib "kernel32" _
   (ByVal hObject As Long) As Long
   
Private Declare Function GetCurrentProcessId _
   Lib "kernel32" () As Long
   
Private Declare Function GetCurrentProcess _
   Lib "kernel32" () As Long
   
Private Declare Function SetPriorityClass Lib "kernel32" _
  (ByVal hProcess As Long, ByVal dwPriorityClass As Long) _
   As Long

    Public Const AVAIL_WAIT = "Please Wait..."
    Public Const AVAIL_START = "Start Working"
    Public Const AVAIL_PAUSE = "Take a Pause"
    Public Const INCOMING_STUDENT = "Incoming Student Call..."
    Public Const LINENUM = 0
    Public Const ID_PRODUCT = 1
    Public Const SC_PRODUCT = 2
    Public Const BC_PRODUCT = 3
    
    Public Recording As Boolean
    Public VaxInitialized As Boolean

   Public Const REG_OPTION_NON_VOLATILE = 0
Public Declare Function SetForegroundWindow Lib "user32" (ByVal hWnd As Long) As Long

Public Declare Function sndPlaySound Lib "winmm" Alias "sndPlaySoundA" (ByVal lpszSoundName As String, ByVal uFlags As Long) As Long
Public Enum SND_Settings
   SND_SYNC = &H0
   SND_ASYNC = &H1
   SND_NODEFAULT = &H2
   SND_MEMORY = &H4
   SND_LOOP = &H8
   SND_NOSTOP = &H10
End Enum


'Constants for topmost.
Public Const HWND_TOPMOST = -1
Public Const HWND_NOTOPMOST = -2
Public Const SWP_NOMOVE = &H2
Public Const SWP_NOSIZE = &H1
Public Const SWP_NOACTIVATE = &H10
Public Const SWP_SHOWWINDOW = &H40
Public Const FLAGS = SWP_NOMOVE Or SWP_NOSIZE

Public Declare Function SetWindowPos Lib "user32" _
   (ByVal hWnd As Long, ByVal hWndInsertAfter As Long, _
    ByVal x As Long, ByVal Y As Long, _
    ByVal cx As Long, ByVal cy As Long, _
    ByVal wFlags As Long) As Long

  
Sub Main()

If App.PrevInstance Then
    MsgBox "A Previous Instance of the Idapted Trainer Client is running"
    End
End If

'Check for Flash
CheckForFlash



log_it "Loading Settings..."
LoadSettings
Debug.Print "load settings"
log_it "Changing priority..."
'ChangePriority (HIGH_PRIORITY_CLASS)

log_it "Showing Main Form..."

frmMain.Show

log_it "Checking Flash Version..."
If ((Not GetFlashMajorVersion = 10) And Not (GetFlashMajorVersion = 9 And GetFlashMinorVersion() >= 115)) Then
  MsgBox "You do not have the latest version of the Adobe Flash player installed on your system (9,0,115,0 or above). For security purposes, we require you to update your flash version. When you press 'ok', your web browser will automatically be directed to the adobe website where you can download the latest version."
  RedirectToFlashInstall
  End
End If
Randomize
'Generate flash unique ide
FlashUniqueId = Int(Rnd * 1000000)

Unloading = False

log_it "ShowingLogin Form..."
frmLogin.Show vbModal
End Sub
Public Sub StopFlashLoading()
   frmMain.FlashLoad.LoadMovie -1, " "
   frmMain.FlashLoad2.LoadMovie -1, " "
End Sub
Public Sub CheckForFlash()

On Error GoTo err_ln
Dim swcontrol As Object
 Set swcontrol = CreateObject("ShockwaveFlash.ShockwaveFlash.9")
Exit Sub
err_ln:
  MsgBox "Adobe Flash Player 9 is not installed.  When you press OK you will be directed to the installation page and the trainer client will exit.  Simply relaunch the trainer client once you have installed flash."
  RedirectToFlashInstall
  End
End Sub
Public Sub RedirectToFlashInstall()
 LaunchIE "http://www.adobe.com/shockwave/download/download.cgi?P1_Prod_Version=ShockwaveFlash"
End Sub
  
Public Function GetFlashMinorVersion()
    Dim version As String, parts() As String
    version = frmMain.FlashLoad.GetVariable("$version")
    parts = Split(version, ",")
    GetFlashMinorVersion = CInt(parts(2))
End Function
Public Function GetFlashMajorVersion()
    Dim version As String, parts() As String
    version = frmMain.FlashLoad.GetVariable("$version")
    parts = Split(version, ",")
    GetFlashMajorVersion = CInt(Split(parts(0), " ")(1))
End Function
Public Function AppVersion()
 AppVersion = CStr(App.major) + "." + CStr(App.minor) + "." + CStr(App.Revision)
End Function
Public Sub ToggleAvailability(avail)
    

    
    If avail = True Then
        frmMain.cmdAvailable.Tag = AVAIL_START
        frmMain.lblStatus.Caption = "Please wait. Unpausing..."
        SendAction "Unpause"
    Else
        frmMain.cmdAvailable.Tag = AVAIL_PAUSE
        frmMain.lblStatus.Caption = "Please wait.  Pausing..."
        SendAction "Pause"
    End If

    frmMain.cmdAvailable.Caption = AVAIL_WAIT
    frmMain.cmdAvailable.Enabled = False
    frmMain.cmdDial.Enabled = False
    frmMain.lblStatus.Tag = AVAIL_WAIT
    

  
End Sub
Public Sub SendAction(action As String)
    Dim json As String
    json = Replace("{'action': '" + action + "'}", "'", Chr(34)) & Chr(10)
    
    If action <> "Ping" Then
        frmMain.Sock.Tag = action
    End If
    frmMain.Sock.SendData json
End Sub
Private Sub LoadSettings()

Dim useProxy As Boolean
Dim product As String
useProxy = False
'Do first run initialization
'If GetKey("SipProxy") = "" Then
'     frmChooseCountry.Show vbModal
'      If frmChooseCountry.lstRegion.List(frmChooseCountry.lstRegion.ListIndex) = "Mainland China" Then useProxy = True
'      product = GetOrCreateRegKey("Product", frmChooseCountry.cmbProduct.ItemData(frmChooseCountry.cmbProduct.ListIndex))
'End If
'
'If GetKey("Product") = BC_PRODUCT Then
'    LoadfrmAdvanceSettingsBC (useProxy)
'ElseIf GetKey("Product") = SC_PRODUCT Then
'    LoadfrmAdvanceSettingsSC (useProxy)
'Else
'  LoadfrmAdvanceSettings
'End If
'
LoadfrmAdvanceSettings
   frmMain.mnuLog.Checked = GetOrCreateRegKey("Log", False)
   
   frmLogin.txtUsername = GetOrCreateRegKey("Username", "")
   
   AudioInDev = GetOrCreateRegKey("AudioInDev", 0)
   AudioOutDev = GetOrCreateRegKey("AudioOutDev", 0)
   
   PromptScreenWidth = GetOrCreateRegKey("PromptScreenWidth", 0)
   PromptScreenHeight = GetOrCreateRegKey("PromptScreenHeight", 0)
   PromptScreenTop = GetOrCreateRegKey("PromptScreenTop", 0)
   PromptScreenLeft = GetOrCreateRegKey("PromptScreenLeft", 0)
   PromptScreenState = GetOrCreateRegKey("PromptScreenState", 2)
   
   AlreadyCompletedEchoTest = False
    
End Sub


Public Sub LoadfrmAdvanceSettings()
Dim ListeningPort As String
  With frmAdvanceLogin
    'Special logic to make change default sip port setting
    .txtSocketURL = GetOrCreateRegKey("SocketURL", "voip.idapted.com")
    .txtSocketPort = GetOrCreateRegKey("SocketPort", 7000)
    .txtAppRoot = GetOrCreateRegKey("AppRoot", "www.idapted.com")
    .txtRealtimeURL = GetOrCreateRegKey("RealtimeURL", "realtime.idapted.com")
    .txtRealtimePort = GetOrCreateRegKey("RealtimePort", 7000)
    .txtLocalVoIPPort = GetOrCreateRegKey("LocalVoIPPort", 7070)
    .txtRTPMin = GetOrCreateRegKey("RTPMin", 16000)
    .txtRTPMax = GetOrCreateRegKey("RTPMax", 65000)
    
   End With
End Sub
Public Sub PreLoadFlashFiles()
    'Preload Flash
   frmMain.FlashLoad.LoadMovie -1, FlashInteractionUrl
   frmMain.FlashLoad2.LoadMovie -1, FlashReviewUrl

End Sub

Public Function FlashInteractionUrl()
Dim url As String
url = frmAdvanceLogin.txtAppRoot
url = RemoveHttp(url)
 FlashInteractionUrl = "http://" + url + "/flex/interaction/trainer/interaction.swf?" & CStr(FlashUniqueId)
 log_it FlashInteractionUrl
End Function

Public Function FlashReviewUrl()
  Dim url As String
  url = frmAdvanceLogin.txtAppRoot
  url = RemoveHttp(url)
  log_it "flash review"
  log_it url
   FlashReviewUrl = "http://" & url & "/flex/markspot/markspot.swf?" & CStr(FlashUniqueId)
   log_it FlashReviewUrl
End Function

Public Function RemoveHttp(url As String)
  If Len(url) < 8 Then
    RemoveHttp = url
    Exit Function
  End If
  If (InStr(1, url, "http") = 1) Or (InStr(1, url, "HTTP") = 1) Then
    RemoveHttp = Right(url, Len(url) - 7)
  Else
    RemoveHttp = url
   End If
End Function




Public Sub SetLoginStatus(status)
    frmLogin.lblStatus.Caption = status
    DoEvents
End Sub
Public Sub SetLoginError(err)
log_it "Login Error" & err
'On Error Resume Next
  frmMain.oVoIP.Hangup
  
  Logoff
  
  Screen.MousePointer = 0
  
  frmMain.Sock.Close
  frmMain.oVoIP.UnInitVELibrary
     frmLogin.fraLogin.Visible = True
     frmLogin.frmStatus.Visible = False
     frmLogin.lblError = err
     frmMain.fraMenu.Visible = True
     frmMain.cmdAvailable.Enabled = True
  
  If frmLogin.Visible = False And frmPromptScreen.Visible = False Then
     If frmAudioTest.Visible = True Then frmAudioTest.Hide
     If frmForcedPause.Visible = True Then frmForcedPause.Hide
     If frmIncoming.Visible = True Then frmIncoming.Hide
     frmLogin.Show vbModal
  ElseIf frmPromptScreen.Visible = True Then
     log_it "Set login error while on prompt screen!"
  End If
 

End Sub





Public Function ChangePriority(dwPriorityClass As _
    PRIORITY_CLASS) As Boolean

'USAGE: Change the Priority of the Current Process
'(i.e., the Process that represents the VB app
'that calls this function)

'PARAMETER: dwPriorityClass: Value as
'defined by the Enum PRIORITY_CLASS in
'declarations

'RETURNS: TRUE IF SUCCESSFUL, FALSE OTHEWISE

'DISCLAIMER: PASSING REALTIME_PRIORITY_CLASS
'TO THIS FUNCTION IS NOT RECOMMENDED

   Dim hProcess As Long
   Dim Ret As Long, pid As Long
   pid = GetCurrentProcess()
   Ret = SetPriorityClass(pid, dwPriorityClass)
       

   ChangePriority = Ret <> 0

End Function
Public Sub UnloadAll()

If Unloading Then Exit Sub
Unloading = True

log_it "unload"
Screen.MousePointer = 11
 log_it "unload logoff"


'On Error Resume Next
 Unload frmLogin
 Unload frmAdvanceLogin
 Unload frmIncoming
 Unload frmAudioTest
 Unload frmTroubleshoot
 Unload frmPromptScreen
 frmMain.oVoIP.Hangup
 frmMain.oVoIP.Logoff
 frmMain.oVoIP.UnInitVELibrary

 
 Unload frmMain
 Screen.MousePointer = 0
 End
End Sub



Public Sub PlayRing()
Dim RetVal As Long
RetVal = sndPlaySound(App.Path + "/ring.wav", SND_LOOP + SND_ASYNC)

End Sub

Public Sub StopRing()
  Dim RetVal As Long
  RetVal = sndPlaySound(vbNullString, SND_ASYNC)
End Sub

Public Function UserIsAdmin()
  If InStr(1, UserType, "admin") > 0 Then
    UserIsAdmin = True
  Else
    UserIsAdmin = False
  End If
End Function



Public Function Quotize(Str) As String
    Quotize = Replace(Str, "'", Chr(34)) + Chr(10)
End Function



Public Sub PauseExec(SecsDelay As Single)
   Dim TimeOut   As Single
   Dim PrevTimer As Single
   
   PrevTimer = Timer
   TimeOut = PrevTimer + SecsDelay
   Do While PrevTimer < TimeOut
      Sleep 4 '-- Timer is only updated every 1/64 sec = 15.625 millisecs.
      DoEvents
      If Timer < PrevTimer Then TimeOut = TimeOut - 86400 '-- pass midnight
      PrevTimer = Timer
   Loop
End Sub




