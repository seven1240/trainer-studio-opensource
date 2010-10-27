VERSION 5.00
Object = "{D27CDB6B-AE6D-11CF-96B8-444553540000}#1.0#0"; "Flash10b.ocx"
Begin VB.Form frmPromptScreen 
   BackColor       =   &H00D09791&
   Caption         =   "Idapted Interaction Screen"
   ClientHeight    =   8475
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10095
   Icon            =   "frmPromptScreen.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   8475
   ScaleWidth      =   10095
   StartUpPosition =   3  'Windows Default
   WindowState     =   2  'Maximized
   Begin VB.Timer tmrReview 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   2640
      Tag             =   "0"
      Top             =   120
   End
   Begin VB.CommandButton cmdReconnect 
      BackColor       =   &H000080FF&
      Caption         =   "Reconnect Student"
      Height          =   375
      Left            =   6000
      Style           =   1  'Graphical
      TabIndex        =   5
      Top             =   0
      Width           =   1935
   End
   Begin VB.Timer tmrReconnect 
      Interval        =   300
      Left            =   7440
      Top             =   120
   End
   Begin ShockwaveFlashObjectsCtl.ShockwaveFlash FlashReview 
      Height          =   8055
      Left            =   0
      TabIndex        =   3
      Top             =   360
      Width           =   10095
      _cx             =   17806
      _cy             =   14208
      FlashVars       =   ""
      Movie           =   ""
      Src             =   ""
      WMode           =   "Window"
      Play            =   "-1"
      Loop            =   "-1"
      Quality         =   "High"
      SAlign          =   ""
      Menu            =   "-1"
      Base            =   ""
      AllowScriptAccess=   ""
      Scale           =   "ShowAll"
      DeviceFont      =   "0"
      EmbedMovie      =   "0"
      BGColor         =   ""
      SWRemote        =   ""
      MovieData       =   ""
      SeamlessTabbing =   "1"
      Profile         =   "0"
      ProfileAddress  =   ""
      ProfilePort     =   "0"
      AllowNetworking =   "all"
      AllowFullScreen =   "false"
   End
   Begin ShockwaveFlashObjectsCtl.ShockwaveFlash Flash 
      Height          =   8055
      Left            =   0
      TabIndex        =   2
      Top             =   360
      Width           =   10095
      _cx             =   17806
      _cy             =   14208
      FlashVars       =   ""
      Movie           =   ""
      Src             =   ""
      WMode           =   "Window"
      Play            =   "-1"
      Loop            =   "-1"
      Quality         =   "High"
      SAlign          =   ""
      Menu            =   "-1"
      Base            =   ""
      AllowScriptAccess=   ""
      Scale           =   "ShowAll"
      DeviceFont      =   "0"
      EmbedMovie      =   "0"
      BGColor         =   ""
      SWRemote        =   ""
      MovieData       =   ""
      SeamlessTabbing =   "1"
      Profile         =   "0"
      ProfileAddress  =   ""
      ProfilePort     =   "0"
      AllowNetworking =   "all"
      AllowFullScreen =   "false"
   End
   Begin VB.CommandButton cmdDisconnect 
      BackColor       =   &H000000FF&
      Caption         =   "Disconnect"
      Height          =   375
      Left            =   8040
      MaskColor       =   &H00D09791&
      Style           =   1  'Graphical
      TabIndex        =   1
      TabStop         =   0   'False
      Top             =   0
      Width           =   1935
   End
   Begin VB.Label lblReconnecting 
      Alignment       =   2  'Center
      Caption         =   "Connection Lost.  Reconnecting..."
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   330
      Left            =   3000
      TabIndex        =   4
      Top             =   0
      Width           =   4155
   End
   Begin VB.Label lblTime 
      Alignment       =   2  'Center
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   330
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Width           =   2235
   End
End
Attribute VB_Name = "frmPromptScreen"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim finished As Boolean

Private Sub cmdDisconnect_Click()
    Dim doDisconnect As Boolean
    Dim re As VbMsgBoxResult
    'If there is more than 30 seconds left,verify that the trainer wants to disconnect
    
    Dim tm As Long
    'Static t As Long
    tm = 900 - DateDiff("s", frmMain.tmrInteraction.Tag, Time)
    If tm > 30 Then
       re = MsgBox("Are you sure you want to disconnect?", vbYesNo + vbDefaultButton2)
       If re = vbYes Then
            doDisconnect = True
        Else
            doDisconnect = False
        End If
    Else
        doDisconnect = True
    End If
    
    If doDisconnect Then
        frmMain.oVoIP.Hangup
    Else
        Me.SetFocus
    End If
End Sub

Private Sub cmdReconnect_Click()
 Dim re As VbMsgBoxResult
 re = MsgBox("This will attempt to re-connect you to the student and fix sound issues.  The student will be disconnected and re-called.  Please do not hit this button twice.  This re-connect may take a few seconds.  Are you sure you want to continue?", vbYesNo)
 If re = vbYes Then
    Dim json As String
    json = Replace("{'action': 'Reconnect','interaction_id': '" + Me.Tag + "'}", "'", Chr(34))
    frmMain.Sock.Tag = action
    frmMain.Sock.SendData json
 End If
End Sub

Private Sub Flash_FSCommand(ByVal command As String, ByVal args As String)

 If command = "saved" Or command = "committedProblems" Then
    finished = True
    Unload Me
ElseIf command = "log" Then
    log_it args
End If
End Sub


Private Sub FlashReview_FSCommand(ByVal command As String, ByVal args As String)
log_it "FS COMMAND"
log_it command

 If command = "saved" Or command = "committedProblems" Then
    FinishedWithPromptScreen = True
    Unload Me
ElseIf command = "log" Then
    log_it "FlashReview"
    log_it args
End If
End Sub

Private Sub Form_Load()
    If Me.Visible = True Then Flash.SetFocus
    FinishedWithPromptScreen = False
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
If FinishedWithPromptScreen = False Then
    Dim re As VbMsgBoxResult
   If Me.Visible = True Then
        re = MsgBox("You should disconnect by clicking the DISCONNECT (interaction) or FINISH (review) button, not closing this window.  Closing this window will cause problems for the student.  Are you sure you want to continue?", vbYesNo)
        If re = vbNo Then
            Cancel = True
        Else
            frmMain.oVoIP.Hangup
        End If
    Else
        frmMain.oVoIP.Hangup
    End If
End If
End Sub

Private Sub Form_Resize()
 If Me.WindowState = 1 Then Exit Sub
  Flash.Width = Me.ScaleWidth
  Flash.Height = Me.ScaleHeight - Flash.Top

  FlashReview.Width = Me.ScaleWidth
  FlashReview.Height = Me.ScaleHeight - FlashReview.Top

  cmdDisconnect.Left = Me.ScaleWidth - cmdDisconnect.Width
  cmdReconnect.Left = cmdDisconnect.Left - cmdReconnect.Width - 10

  PromptScreenWidth = Me.Width
  SaveKey "PromptScreenWidth", Me.Width


  PromptScreenHeight = Me.Height
  SaveKey "PromptScreenHeight", Me.Height

  PromptScreenTop = Me.Top
  SaveKey "PromptScreenTop", Me.Top

  PromptScreenLeft = Me.Left
  SaveKey "PromptScreenLeft", Me.Left


  PromptScreenState = Me.WindowState

  SaveKey "PromptScreenState", Me.WindowState
End Sub

Private Sub Form_Unload(Cancel As Integer)

'If Finished = False Then
'    Dim re As VbMsgBoxResult
'
'    re = MsgBox("Closing this window without saving the review will not count this interaction.  Are you sure you want to continue?", vbYesNo)
'    If re = vbYes Then
'        frmMain.oVoIP.Hangup
'        ResetInteraction
'    End If
'End If
 frmMain.oVoIP.Hangup
 tmrReview.Enabled = False
 If Not Unloading Then
    If frmMain.cmdAvailable.Caption = AVAIL_START Then
        ToggleAvailability (False)
    Else
        ToggleAvailability (True)
    End If
End If

End Sub


Private Sub tmrReconnect_Timer()
    lblReconnecting.Visible = True
 If lblReconnecting.BackColor = vbRed Then
      lblReconnecting.BackColor = vbWhite
Else
    lblReconnecting.BackColor = vbRed
 End If
End Sub

Private Sub tmrReview_Timer()
Dim tm As Long
'Static t As Long
tm = DateDiff("s", tmrReview.Tag, Time)
't = t + 1
'tm = t

  'Set the color of the timer view
  Dim m As Long
  Dim s As Long
  Dim neg As Boolean
  Dim Str As String
  Str = "Rev. Time: "
  m = tm \ 60
  s = tm Mod 60
  
  
  If tm < 100 Then
    frmPromptScreen.lblTime.BackColor = vbGreen
  ElseIf tm < 120 Then
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
