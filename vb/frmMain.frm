VERSION 5.00
Object = "{D27CDB6B-AE6D-11CF-96B8-444553540000}#1.0#0"; "Flash.ocx"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "comctl32.ocx"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Object = "{013F70BC-8C47-4BDF-B295-AB5ADB695696}#1.0#0"; "cSystemInfoAX.ocx"
Begin VB.Form frmMain 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Idapted Trainer Client"
   ClientHeight    =   5580
   ClientLeft      =   150
   ClientTop       =   840
   ClientWidth     =   3030
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5580
   ScaleWidth      =   3030
   StartUpPosition =   3  '´°¿ÚÈ±Ê¡
   Begin VB.Timer tmrCallQuality 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6600
      Top             =   840
   End
   Begin System_Info_ActiveX.cSysInformation SysInfo 
      Left            =   6600
      Top             =   2880
      _ExtentX        =   900
      _ExtentY        =   900
   End
   Begin MSWinsockLib.Winsock Sock 
      Left            =   4440
      Top             =   3600
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
   End
   Begin VB.Frame frmDisplay 
      BorderStyle     =   0  'None
      Height          =   2205
      Left            =   45
      TabIndex        =   22
      Top             =   3375
      Visible         =   0   'False
      Width           =   2940
      Begin VB.Timer tmrLevels 
         Enabled         =   0   'False
         Interval        =   100
         Left            =   2310
         Top             =   1620
      End
      Begin VB.Frame Frame1 
         Caption         =   "Microphone"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1050
         Left            =   30
         TabIndex        =   28
         Top             =   1095
         Width           =   2835
         Begin ComctlLib.ProgressBar MicProgress 
            Height          =   210
            Left            =   645
            TabIndex        =   29
            Top             =   285
            Width           =   1440
            _ExtentX        =   2540
            _ExtentY        =   370
            _Version        =   327682
            Appearance      =   1
            Max             =   9
         End
         Begin ComctlLib.Slider SliderMicVol 
            Height          =   255
            Left            =   750
            TabIndex        =   30
            Top             =   645
            Width           =   1980
            _ExtentX        =   3493
            _ExtentY        =   450
            _Version        =   327682
            Max             =   25
         End
         Begin VB.Label Label2 
            Caption         =   "Volume"
            Height          =   225
            Index           =   2
            Left            =   75
            TabIndex        =   32
            Top             =   600
            Width           =   540
         End
         Begin VB.Label Label1 
            Caption         =   "Level"
            Height          =   240
            Index           =   1
            Left            =   90
            TabIndex        =   31
            Top             =   285
            Width           =   480
         End
      End
      Begin VB.Frame frmSpeaker 
         Caption         =   "Headphones"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1005
         Left            =   0
         TabIndex        =   23
         Top             =   0
         Width           =   2865
         Begin ComctlLib.ProgressBar SpkProgress 
            Height          =   210
            Left            =   645
            TabIndex        =   24
            Top             =   285
            Width           =   1470
            _ExtentX        =   2593
            _ExtentY        =   370
            _Version        =   327682
            Appearance      =   1
            Max             =   9
         End
         Begin ComctlLib.Slider SliderSpkVol 
            Height          =   255
            Left            =   750
            TabIndex        =   25
            Top             =   630
            Width           =   1980
            _ExtentX        =   3493
            _ExtentY        =   450
            _Version        =   327682
            Max             =   25
         End
         Begin VB.Label Label2 
            Caption         =   "Volume"
            Height          =   225
            Index           =   0
            Left            =   105
            TabIndex        =   27
            Top             =   645
            Width           =   540
         End
         Begin VB.Label Label1 
            Caption         =   "Level"
            Height          =   240
            Index           =   0
            Left            =   105
            TabIndex        =   26
            Top             =   300
            Width           =   480
         End
      End
   End
   Begin VB.Frame fraInteraction 
      Height          =   3375
      Left            =   120
      TabIndex        =   20
      Top             =   0
      Visible         =   0   'False
      Width           =   2775
      Begin VB.Timer tmrKeepAlive 
         Enabled         =   0   'False
         Interval        =   10000
         Left            =   1800
         Top             =   1560
      End
      Begin ShockwaveFlashObjectsCtl.ShockwaveFlash FlashLoad2 
         Height          =   30
         Left            =   1080
         TabIndex        =   34
         Top             =   1680
         Visible         =   0   'False
         Width           =   495
         _cx             =   873
         _cy             =   53
         FlashVars       =   ""
         Movie           =   ""
         Src             =   ""
         WMode           =   "Window"
         Play            =   -1  'True
         Loop            =   -1  'True
         Quality         =   "High"
         SAlign          =   ""
         Menu            =   -1  'True
         Base            =   ""
         AllowScriptAccess=   ""
         Scale           =   "ShowAll"
         DeviceFont      =   0   'False
         EmbedMovie      =   0   'False
         BGColor         =   ""
         SWRemote        =   ""
         MovieData       =   ""
         SeamlessTabbing =   -1  'True
         Profile         =   0   'False
         ProfileAddress  =   ""
         ProfilePort     =   0
         AllowNetworking =   "all"
         AllowFullScreen =   "false"
      End
      Begin ShockwaveFlashObjectsCtl.ShockwaveFlash FlashLoad 
         Height          =   135
         Left            =   2160
         TabIndex        =   33
         Top             =   3000
         Visible         =   0   'False
         Width           =   495
         _cx             =   873
         _cy             =   238
         FlashVars       =   ""
         Movie           =   ""
         Src             =   ""
         WMode           =   "Window"
         Play            =   -1  'True
         Loop            =   -1  'True
         Quality         =   "High"
         SAlign          =   ""
         Menu            =   -1  'True
         Base            =   ""
         AllowScriptAccess=   ""
         Scale           =   "ShowAll"
         DeviceFont      =   0   'False
         EmbedMovie      =   0   'False
         BGColor         =   ""
         SWRemote        =   ""
         MovieData       =   ""
         SeamlessTabbing =   -1  'True
         Profile         =   0   'False
         ProfileAddress  =   ""
         ProfilePort     =   0
         AllowNetworking =   "all"
         AllowFullScreen =   "false"
      End
      Begin VB.Timer tmrRing 
         Enabled         =   0   'False
         Interval        =   3000
         Left            =   2280
         Top             =   2160
      End
      Begin VB.Timer tmrInteraction 
         Enabled         =   0   'False
         Interval        =   1000
         Left            =   1800
         Tag             =   "0"
         Top             =   840
      End
      Begin VB.Label Label2 
         Alignment       =   2  'Center
         Caption         =   "Connected to Student"
         BeginProperty Font 
            Name            =   "Arial Black"
            Size            =   12
            Charset         =   0
            Weight          =   900
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   615
         Index           =   1
         Left            =   240
         TabIndex        =   21
         Top             =   240
         Width           =   2175
      End
   End
   Begin VB.Frame fraMenu 
      Height          =   3375
      Left            =   120
      TabIndex        =   15
      Top             =   0
      Width           =   2775
      Begin VB.CommandButton cmdDial 
         Caption         =   "Dial Number"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   240
         TabIndex        =   17
         Top             =   2640
         Width           =   2295
      End
      Begin VB.CommandButton cmdAvailable 
         Caption         =   "Start Training"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   240
         TabIndex        =   16
         Top             =   2160
         Width           =   2295
      End
      Begin VB.Label lblRed 
         Alignment       =   2  'Center
         BackColor       =   &H000000FF&
         Caption         =   "Label3"
         Height          =   255
         Left            =   120
         TabIndex        =   35
         Top             =   240
         Visible         =   0   'False
         Width           =   2535
      End
      Begin VB.Label lblStatus 
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1815
         Left            =   240
         TabIndex        =   18
         Top             =   240
         Width           =   2295
      End
   End
   Begin VB.Frame fraDial 
      Height          =   3375
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Visible         =   0   'False
      Width           =   2775
      Begin VB.CommandButton cmdMute 
         Caption         =   "Mute"
         Height          =   255
         Left            =   360
         TabIndex        =   14
         Top             =   240
         Width           =   1815
      End
      Begin VB.CommandButton cmdHangup 
         Caption         =   "Hangup"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   240
         TabIndex        =   13
         TabStop         =   0   'False
         Top             =   2520
         Width           =   2175
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "1"
         Height          =   375
         Index           =   1
         Left            =   120
         TabIndex        =   12
         Top             =   600
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "2"
         Height          =   375
         Index           =   2
         Left            =   960
         TabIndex        =   11
         Top             =   600
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "3"
         Height          =   375
         Index           =   3
         Left            =   1800
         TabIndex        =   10
         Top             =   600
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "4"
         Height          =   375
         Index           =   4
         Left            =   120
         TabIndex        =   9
         Top             =   1080
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "5"
         Height          =   375
         Index           =   5
         Left            =   960
         TabIndex        =   8
         Top             =   1080
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "6"
         Height          =   375
         Index           =   6
         Left            =   1800
         TabIndex        =   7
         Top             =   1080
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "7"
         Height          =   375
         Index           =   7
         Left            =   120
         TabIndex        =   6
         Top             =   1560
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "8"
         Height          =   375
         Index           =   8
         Left            =   960
         TabIndex        =   5
         Top             =   1560
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "9"
         Height          =   375
         Index           =   9
         Left            =   1800
         TabIndex        =   4
         Top             =   1560
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "*"
         Height          =   375
         Index           =   10
         Left            =   120
         TabIndex        =   3
         Top             =   2040
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "0"
         Height          =   375
         Index           =   0
         Left            =   960
         TabIndex        =   2
         Top             =   2040
         Width           =   735
      End
      Begin VB.CommandButton BtnDialPad 
         Caption         =   "#"
         Height          =   375
         Index           =   11
         Left            =   1800
         TabIndex        =   1
         Top             =   2040
         Width           =   735
      End
      Begin VB.Label lblDialStatus 
         Caption         =   "Label1"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   240
         TabIndex        =   19
         Top             =   3000
         Width           =   2295
      End
   End
   Begin VB.Timer tmrRegister 
      Enabled         =   0   'False
      Interval        =   7000
      Left            =   2160
      Top             =   1920
   End
   Begin VB.Menu mnuFile 
      Caption         =   "File"
      Begin VB.Menu mnuExit 
         Caption         =   "Exit"
      End
   End
   Begin VB.Menu mnuSites 
      Caption         =   "Sites"
      Begin VB.Menu mnuTrainerPortal 
         Caption         =   "Trainer Portal"
      End
      Begin VB.Menu mnuChat 
         Caption         =   "Chat"
      End
   End
   Begin VB.Menu mnuSettings 
      Caption         =   "Settings"
      Begin VB.Menu mnuEchoTest 
         Caption         =   "Echo Test"
      End
      Begin VB.Menu mnuVersion 
         Caption         =   "Version"
      End
      Begin VB.Menu mnuLog 
         Caption         =   "Log Activity"
      End
      Begin VB.Menu GetRTPStats 
         Caption         =   "Get RTP Stats"
      End
      Begin VB.Menu GetRTCPStats 
         Caption         =   "Get RTCP Stats"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public WithEvents oVoIP  As VEObj
Attribute oVoIP.VB_VarHelpID = -1

Private RegistrationRetries As Long
Private SuccessfullyRegistered As Boolean
Private LoadedInteraction As Long

Private LastAvgJitter As Long
Private LastMaxJitter As Long
Private LastDiscardedPackets As Long
Private LastCumLost As Long
Private LastFractionLost As Long
Private ReceivedCallQuality As Boolean


'Private Const proxy = "72.51.42.81"
  Private Const SW_SHOW = 5       ' Displays Window in its current size
                                      ' and position
      Private Const SW_SHOWNORMAL = 1 ' Restores Window if Minimized or

                                      ' Maximized
    

      Private Declare Function ShellExecute Lib "SHELL32.DLL" Alias _
         "ShellExecuteA" (ByVal hWnd As Long, ByVal lpOperation As _
         String, ByVal lpFile As String, ByVal lpParameters As String, _
         ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
         


Private Sub Pause()
 SetStatus "You are logged into the server." + vbCrLf + vbTab + "* To take calls from Students, press 'Start Working'" + vbCrLf + vbTab + "* To dial a number (i.e. dial into a conference) press 'Dial Number'"
 cmdAvailable.Caption = AVAIL_START
 cmdAvailable.Enabled = True
 cmdDial.Enabled = True
End Sub

Private Sub ConnectionSuccess()
    'Make sure we stop the ringing
    Dim url As String
    StopRing
        
        
    'Set Levels
    tmrLevels.Enabled = True
    frmDisplay.Visible = True
    Result = oVoIP.GetSKVolume()
   
    SliderSpkVol.value = Result / 10
    frmAudioTest.SliderSpkVol.value = Result / 10
    Result = oVoIP.GetMicVolume()
    SliderMicVol.value = Result / 10
    frmAudioTest.SliderMicVol.value = Result / 10
    
  '  cmdMuteSpkr.Caption = "Mute"
  '  cmdMuteMic.Caption = "Mute"
    
   
    lblDialStatus.Caption = "Connected"
    
  'If this is interaction call...load stuff up
    If frmIncoming.Visible = True And frmIncoming.Caption = INCOMING_STUDENT Then
        frmIncoming.Hide
        log_it "Asking call with flashvars"
        log_it frmIncoming.Tag
  

        'Resize prompt screen
        If PromptScreenState = 2 Then
            frmPromptScreen.WindowState = 2
        ElseIf PromptScreenState = 0 Then
 
            frmPromptScreen.WindowState = 0
            If PromptScreenWidth > 100 And PromptScreenHeight > 100 And PromptScreenLeft > 0 And PromptScreenLeft < Screen.Width And PromptScreenTop > 0 And PromptScreenTop < Screen.Height Then
                frmPromptScreen.Move PromptScreenLeft, PromptScreenTop, PromptScreenWidth, PromptScreenHeight
            End If
            
        End If
            frmPromptScreen.cmdDisconnect.Visible = True
            frmPromptScreen.cmdReconnect.Visible = True
    frmPromptScreen.lblTime.Visible = True
           frmPromptScreen.Show
        frmPromptScreen.Flash.SetFocus
        frmPromptScreen.lblReconnecting.Visible = False
        frmPromptScreen.tmrReconnect.Enabled = False
        frmPromptScreen.tmrReview.Enabled = False
         
         frmMain.tmrInteraction.Tag = Time


        frmMain.fraMenu.Visible = False
        frmMain.fraInteraction.Visible = True
        frmMain.tmrInteraction.Enabled = True

    ElseIf EchoTest > 0 Then
        EchoTestConnect
    Else
        frmIncoming.Hide
        fraDial.Visible = True
        fraMenu.Visible = False
        lblDialStatus.Caption = "Connected"

    End If
End Sub

Private Sub HungUp()
 log_it "HungUp"
 
 fraDial.Visible = False
 fraMenu.Visible = True
 tmrLevels.Enabled = False
 frmDisplay.Visible = False
tmrInteraction.Enabled = False
 fraInteraction.Visible = False
 'Make sure flash continues to load
 PreLoadFlashFiles


 
If EchoTest > 0 Then
    EndTest
End If

 'Disconnected from an interaction with student
 If frmPromptScreen.Visible = True Then
    Disconnect
End If

End Sub



Private Sub cmdAvailable_Click()
    If cmdAvailable.Caption = AVAIL_START Then
     
        ToggleAvailability (True)
    Else
        ToggleAvailability (False)
    End If

End Sub







Sub SetStatus(status)
lblStatus.Caption = status
End Sub


Private Sub BtnDialPad_Click(index As Integer)
   
    oVoIP.SendDTMF index
 
End Sub

Private Sub cmdDial_Click()

If cmdAvailable.Caption = AVAIL_PAUSE And Not UserIsAdmin Then
    MsgBox "You must pause training before placing a call..."
    Exit Sub
End If
    Dim num As String
  num = InputBox("Please enter the number to dial")
  If num = "" Then Exit Sub
  DialNumber num
End Sub

Private Sub cmdHangup_Click()
    oVoIP.Hangup
    fraDial.Visible = False
    fraMenu.Visible = True
End Sub

Private Sub cmdMute_Click()
If cmdMute.Caption = "Mute" Then
    VaxSIPUserAgentOCX.MuteMic True
    cmdMute.Caption = "UnMute"
Else
    cmdMute.Caption = "Mute"
    VaxSIPUserAgentOCX.MuteMic False
End If
End Sub









Private Sub Command1_Click()
oVoIP.Logoff

End Sub

Private Sub FlashLoad_OnReadyStateChange(newState As Long)
log_it "Readstate change"
log_it newState
End Sub


Private Sub Form_Load()
Set oVoIP = New VEObj
SysInfo.Init "bs68d741s8-caw98789w-nf98t7-av91w87"
cmdAvailable.Caption = AVAIL_PAUSE
ReceivedCallQuality = False
End Sub


Private Sub Form_Unload(Cancel As Integer)

UnloadAll
End Sub



Private Sub GetRTCPStats_Click()
oVoIP.GetRTCPStat
End Sub

Private Sub GetRTPStats_Click()
oVoIP.GetRTPStat
End Sub


Private Sub mnuEchoTest_Click()
If cmdAvailable.Caption = AVAIL_PAUSE Then
  MsgBox "You must pause yourself before starting the echo test"
Else
    blnExitOnCloseTest = False
    ShowAudioTest
End If
End Sub

Private Sub mnuLog_Click()
 Dim val As Boolean
 val = mnuLog.Checked
 val = Not val
 mnuLog.Checked = val
 SaveKey "log", val
End Sub

Private Sub oVoIP_InComingCall(ByVal strPhoneLine As String, ByVal strCallerID As String, ByVal strDisplayName As String)

log_it "OnIncoming Call strPhoneLine: " & CStr(strPhoneLine) & " strCallerID: " & CStr(strCallerID) & " strDisplayName: " & strDisplayName

If frmPromptScreen.Visible = False And frmIncoming.Visible = False And frmLogin.Visible = False And frmForcedPause.Visible = False Then
    log_it "On Incoming Call Show frmIncoming..."
    frmIncoming.Caption = INCOMING_STUDENT
    If InStr(1, strDisplayName, "IT") = 2 Then
        frmIncoming.Tag = Mid(strDisplayName, 4, Len(strDisplayName) - 4)
  log_it "TAG"
  log_it frmIncoming.Tag
         If Not ReceivedInteractionInfo(frmIncoming.Tag) Then
            log_it "Do not have interaction info for: " + frmIncoming.Tag
            frmMain.oVoIP.RejectInboundCall
            Exit Sub
        End If
        
    
    
    Else
        frmIncoming.Caption = "Incoming Call From: " & strDisplayName & "/" & strCallerID
        frmIncoming.Tag = strCallerID
    End If
  
    
    PlayRing

    
    frmIncoming.Show vbModal
Else
    log_it "OnIncoming Call Reject"
    frmMain.oVoIP.RejectInboundCall
End If

End Sub

Private Sub oVoIP_RegistrationStatus(ByVal strDID As String, ByVal lType As Long)
Debug.Print "Registration Status: " & CStr(lType)

If lType = 1 Then
  'Registration Successful
    SuccessfullyRegistered = True
    lblRed.Visible = False
    If frmLogin.Visible = True Then
       frmLogin.Hide
       blnExitOnCloseTest = True
       ShowAudioTest
    ElseIf RegistrationRetries > 0 Then
        'Success re-registration
         RegistrationRetries = 0
'        If cmdAvailable.Caption = AVAIL_START Then
'            ToggleAvailability (False)
'        Else
'            ToggleAvailability (True)
'        End If
    Else
       log_it "Re-registered"
    End If
    
ElseIf lType = 0 Then
    'Unregistration after purposeful un-registration
    log_it "Unregistered"
Else

    
    
    log_it "Fail to register: " & CStr(lType)
    If Not frmPromptScreen.Visible = True Then
        cmdAvailable.Caption = AVAIL_START

    
         If lType = 403 Then
            SetLoginError "Error logging in!  Invalid Password"
        ElseIf lType = 404 Then
            SetLoginError "Error logging in!  Invalid Username"
        ElseIf lType = 408 Then
            log_it "got unregistered, trying to register"
            'Retry once automatically.  If that fails, then throw error.
            If RegistrationRetries > 2 Then
                RegistrationRetries = 0
                SetLoginError "Your connection to the VoIP Server appears unstable.  Please check your connection and try again."
            Else
                RegistrationRetries = RegistrationRetries + 1
                log_it "Retrying Registration: " + CStr(RegistrationRetries)
                Login
            End If
        Else
            SetLoginError "Error logging in!  Unknown Error: " & CStr(lType)
        End If
    Else
       'Re-register automatically.
         frmMain.oVoIP.SetDIDsList VoipLogin
        
    End If
End If
End Sub

Private Sub oVoIP_RTCPStat(ByVal lFraction_lost As Long, ByVal lCum_lost As Long, ByVal lExt_max As Long, ByVal lJitter As Long, ByVal lRTT As Long)
Dim s As String
s = "rtcpstat, fraction_lost: " & CStr(lFraction_lost / 255) & " cum_lost: " & CStr(lCum_lost) & " ext_max: " & CStr(lExt_max) & " ljitter: " & CStr(lJitter) & " rtt: " & CStr(lRTT)


LastFractionLost = lFraction_lost
LastCumLost = lCum_lost
ReceivedCallQuality = True
End Sub

Private Sub oVoIP_RTPStat(ByVal lAvg_jitter_MS As Long, ByVal lMax_jitter_MS As Long, ByVal lDiscardedPackets As Long)
Dim s As String
s = "rtp stat, avg_jiter: " & CStr(lAvg_jitter_MS) & " lmax jitter: " & CStr(lMax_jitter_MS) & " discardedpackets: " & CStr(lDiscardedPackets)


LastAvgJitter = lAvg_jitter_MS
LastMaxJitter = lMax_jitter_MS
LastDiscardedPackets = lDiscardedPackets
End Sub

Private Sub oVoIP_SessionError(ByVal lErrorID As Long)
If lErrorID = IDS_STUN_ERRR Then
    log_it "STUN ERROR"
'    SetLoginError "Cannot connect to STUN server.  Please check your firewall."
ElseIf lErrorID = IDS_SYMMETRIC_NAT Then
    log_it "SYMMETRIC NAT"
'    SetLoginError "You are behind a symmetric NAT.  Our voip system does not support symmetric NAT.  Please contact trainer support for solutions."
ElseIf lErrorID = IDS_FIREWALL Then
    SetLoginError "You are behind a firewall that is preventing proper access to the Idapted Voice Network.  Please allow the trainer client access through the firewall."
Else
    MsgBox "session error"
    MsgBox lErrorID
End If
End Sub

Private Sub oVoIP_SessionStateChanged(ByVal lStateID As Long)
log_it "state changed: " & CStr(lStateID)
'MsgBox lStateID
Select Case lStateID
    Case ID200CODE
        'Connected successfully
        tmrCallQuality.Enabled = True
         ConnectionSuccess
    Case ID200BYECODE
        'Call hungup
        SendCallQuality
        HungUp
    Case NOT_FOUND_MSG
        'Tried to dial invalid number
        oVoIP.Hangup
        fraDial.Visible = False
        fraMenu.Visible = True
        MsgBox "Error.  The number you are dialing cannot be found.", vbExclamation
    Case INBOUNDCANCELCODE
        'Incoming call was cancelled
        If frmForcedPause.Visible = False Then
            frmIncoming.Hide
        End If
        StopRing
    Case IDLECODE
        HungUp
        
End Select


End Sub



Private Sub SliderMicVol_Click()
    
    frmMain.oVoIP.SetMixerVolume 9, (SliderMicVol.value * 10)
        
   
End Sub

Private Sub SliderSpkVol_Click()


    frmMain.oVoIP.SetMixerVolume 8, (SliderSpkVol.value * 10)
    
End Sub


Private Sub mnuChat_Click()
ShellExecute Me.hWnd, vbNullString, frmAdvanceLogin.txtAppRoot + "/trainer_chat", vbNullString, vbNullString, SW_SHOWNORMAL

End Sub

Private Sub mnuExit_Click()
UnloadAll
End Sub



Private Sub mnuTrainerPortal_Click()
ShellExecute Me.hWnd, vbNullString, "http://trainer.idapted.com", vbNullString, vbNullString, SW_SHOWNORMAL
End Sub

Private Sub mnuVersion_Click()
MsgBox "Idapted Trainer Client Version: " & AppVersion
End Sub

Private Sub Sock_Close()
    Debug.Print "CLOSE"
    MsgBox "closeing"
    tmrKeepAlive.Enabled = False
    If Sock.Tag = "shutdown" Then
      MsgBox "shutdown2"
      UnloadAll
    Else
        SetLoginError ("Lost connection to server.  Please try to relogin!")
    End If
    
End Sub

Private Sub Sock_Connect()
 SocketAuthenticate
End Sub

Private Sub Sock_DataArrival(ByVal bytesTotal As Long)
On Error GoTo err

Dim data As String, json As New json
Sock.GetData data

log_it "GOT DATA with Sock.Tag: " + Sock.Tag
log_it data

json.Decode data


Select Case json.Fetch("status")

    Case "Unregistered":
        lblRed.Caption = "Lost connection to VoIP Server.  Re-logging in."
        lblRed.Visible = True
        log_it "Registered based on status"
        frmMain.oVoIP.SetDIDsList VoipLogin
        log_it "UNREGISTER STATUS"
    Case "ForcedPause":
        Pause
        flash_taskbar_icon (Me.hWnd)
           frmIncoming.Hide 'Make sure its hidden
           frmForcedPause.lblReason = json.Fetch("reason")
           If frmIncoming.Visible = True Then
                oVoIP.RejectInboundCall
                frmIncoming.Hide
           End If
           frmForcedPause.Show vbModal
    Case "Message"
            log_it "MESSAGE"
            If frmPromptScreen.Visible = True And frmPromptScreen.FlashReview.Visible = True Then
                Dim invoke As String
                invoke = "<invoke name=" & Chr(34) & "showMsg" & Chr(34) & " returntype=" & Chr(34) & "xml" & Chr(34) & "><arguments><string>" + json.Fetch("message") + "</string></arguments></invoke>"
                log_it invoke
                frmPromptScreen.FlashReview.CallFunction (invoke)
            End If
    Case "ReservedForInteraction":
        PreLoadInteractionScreen json.Fetch("interaction_id"), json.Fetch("scenario_id"), json.Fetch("realtime_uuid")
        
    Case Else
        Select Case json.Fetch("action")
        Case "LostConnection":
            frmPromptScreen.tmrReconnect.Enabled = True
            frmPromptScreen.cmdReconnect.Visible = False
        Case "Reconnected":
            frmPromptScreen.lblReconnecting.Visible = False
            frmPromptScreen.cmdReconnect.Visible = True
            frmPromptScreen.tmrReconnect.Enabled = False
        Case Else
            
            Select Case Sock.Tag
            Case "Pause":
               log_it "Pause on received JSON"
               Pause
               flash_taskbar_icon (frmMain.hWnd)
             Case "Unpause":
                SetStatus "Available as Trainer." + vbCrLf + "Waiting for Students..."
                cmdAvailable.Caption = AVAIL_PAUSE
                cmdAvailable.Enabled = True
                unflash_taskbar_icon (frmMain.hWnd)

            Case "Authenticate":
               If Not LoginToVoip(json) Then
                  SetLoginError "You must download the new version of the Trainer Client to login"
               End If
               
               tmrKeepAlive.Enabled = True
            End Select
            
            'reset Sock.Tag to avoid side effect
            Sock.Tag = "Ping"
               
        End Select
    End Select

Exit Sub

err:
SetLoginError ("Error with data received from socket!")


End Sub

Private Sub Sock_Error(ByVal Number As Integer, Description As String, ByVal Scode As Long, ByVal Source As String, ByVal HelpFile As String, ByVal HelpContext As Long, CancelDisplay As Boolean)
    
    SetLoginError "Unable to connect to server.  Either you lost your internet connection or your firewall is blocking this software (Code: " + CStr(Number) + ")"

End Sub

Private Sub tmrCallQuality_Timer()
    oVoIP.GetRTCPStat
    oVoIP.GetRTPStat
End Sub

Private Sub tmrInteraction_Timer()
Dim tm As Long
'Static t As Long
tm = 900 - DateDiff("s", tmrInteraction.Tag, Time)
't = t + 1
'tm = t

  'Set the color of the timer view
  Dim m As Long
  Dim s As Long
  Dim neg As Boolean
  Dim Str As String
  Str = "Call Time: "
  m = tm \ 60
  s = tm Mod 60
  If s < 0 Or m < 0 Then
    s = Abs(s)
    m = Abs(m)
    Str = Str & "- "
  End If
  
  
  If tm > 30 Then
    frmPromptScreen.lblTime.BackColor = vbGreen
  ElseIf tm > 0 Then
    frmPromptScreen.lblTime.BackColor = vbYellow
  Else
    If frmPromptScreen.lblTime.BackColor = vbRed Then
      frmPromptScreen.lblTime.BackColor = vbWhite
    Else
       frmPromptScreen.lblTime.BackColor = vbRed
    End If
  End If
  Str = Str & Format(m, "00") + ":" + Format(s, "00")
  frmPromptScreen.lblTime.Caption = Str
End Sub

Private Sub tmrKeepAlive_Timer()
On Error Resume Next
  SendAction "Ping"

End Sub

Private Sub tmrLevels_Timer()
    Dim mic As Long, spk As Long
    spk = frmMain.oVoIP.GetOutputLevel()
    SpkProgress.value = spk
    frmAudioTest.SpkProgress.value = spk
    
    mic = frmMain.oVoIP.GetInputLevel()

    MicProgress.value = mic
    frmAudioTest.MicProgress.value = mic
    
    If blnDetectLevel Then
        ReDim Preserve DetectedLevels(UBound(DetectedLevels) + 1)
        DetectedLevels(UBound(DetectedLevels)) = mic
    End If
End Sub

Private Sub PreLoadInteractionScreen(InteractionID As Long, ScenarioID As Long, RealtimeUUID As String)
Dim url As String, vars As String
        frmPromptScreen.Flash.LoadMovie 0, "empty.swf"
        DoEvents
        vars = "realtime_host=" & frmAdvanceLogin.txtRealtimeURL & "&realtime_channel=" & RealtimeUUID & "&font_size=12&cs_number=4008871020&realtime_port=2000&interaction_id=" & CStr(InteractionID) & "&scenario_id=" & CStr(ScenarioID) & "&realtime_subscriber=" & frmLogin.txtUsername & "&trainer_login=" & frmLogin.txtUsername & "&trainer_pwd=" & frmLogin.txtPassword & "&base_url=http://" & RemoveHttp(frmAdvanceLogin.txtAppRoot)
        
        frmPromptScreen.Flash.FlashVars = vars
        log_it vars
        'frmPromptScreen.Flash.FlashVars = "host=http://" & url & "&interaction_id=" & frmPromptScreen.Tag & "&socket_server=" & frmAdvanceLogin.txtChatUrl & "&socket_port=1055&customer_service=010-82601610&trainer_login=" & frmLogin.txtUsername & "&password=" & frmLogin.txtPassword & "&product_type=" & frmAdvanceLogin.txtUserDomain
        frmPromptScreen.Flash.LoadMovie 0, FlashInteractionUrl
        frmPromptScreen.Flash.Visible = True
        frmPromptScreen.FlashReview.Visible = False

        frmPromptScreen.Tag = CStr(InteractionID)
        LoadedInteraction = InteractionID
End Sub


Private Function ReceivedInteractionInfo(InteractionID As String)
    If CLng(InteractionID) = LoadedInteraction Then
        ReceivedInteractionInfo = True
    Else
        PauseExec 2#
        If CLng(InteractionID) = LoadedInteraction Then
            ReceivedInteractionInfo = True
        Else
            ReceivedInteractionInfo = False
        End If
    End If
    

End Function

Private Sub SendCallQuality()

    tmrCallQuality.Enabled = False
    If ReceivedCallQuality Then
        frmMain.Sock.SendData Quotize("{'action': 'CallQuality','fraction_lost': '" & LastFractionLost & "','cum_lost': '" & LastCumLost & "','avg_jitter': '" & LastAvgJitter & "','max_jitter': '" & LastMaxJitter & "','discarded_packets': '" & LastDiscardedPackets & "'}")
        ReceivedCallQuality = False
    End If
End Sub

