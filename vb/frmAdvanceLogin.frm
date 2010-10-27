VERSION 5.00
Begin VB.Form frmAdvanceLogin 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Idapted Advance Login Settings"
   ClientHeight    =   5700
   ClientLeft      =   8190
   ClientTop       =   6510
   ClientWidth     =   5190
   Icon            =   "frmAdvanceLogin.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5700
   ScaleWidth      =   5190
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox txtRTPMax 
      Height          =   285
      Left            =   3120
      TabIndex        =   19
      Text            =   "Text2"
      Top             =   4080
      Width           =   735
   End
   Begin VB.TextBox txtRTPMin 
      Height          =   285
      Left            =   2160
      TabIndex        =   18
      Text            =   "Text2"
      Top             =   4080
      Width           =   735
   End
   Begin VB.TextBox txtLocalVoIPPort 
      Height          =   285
      Left            =   2160
      TabIndex        =   15
      Text            =   "txtLocalPort"
      Top             =   3600
      Width           =   1815
   End
   Begin VB.TextBox txtRealtimePort 
      Height          =   285
      Left            =   2160
      TabIndex        =   14
      Text            =   "txtRealtimePort"
      Top             =   2730
      Width           =   1815
   End
   Begin VB.TextBox txtRealtimeURL 
      Height          =   285
      Left            =   2160
      TabIndex        =   13
      Text            =   "txtRealtimeURL"
      Top             =   2190
      Width           =   1815
   End
   Begin VB.TextBox txtAppRoot 
      Height          =   285
      Left            =   2160
      TabIndex        =   12
      Text            =   "txtAppRoot"
      Top             =   1665
      Width           =   1815
   End
   Begin VB.TextBox txtSocketPort 
      Height          =   285
      Left            =   2160
      TabIndex        =   11
      Text            =   "txtSocketPort"
      Top             =   1125
      Width           =   1815
   End
   Begin VB.TextBox txtSocketURL 
      Height          =   285
      Left            =   2160
      TabIndex        =   10
      Text            =   "txtSocketURL"
      Top             =   600
      Width           =   1815
   End
   Begin VB.CommandButton cmdReset 
      Caption         =   "Reset"
      Height          =   255
      Left            =   4080
      TabIndex        =   4
      Top             =   120
      Width           =   855
   End
   Begin VB.ComboBox cmbPresets 
      Height          =   315
      ItemData        =   "frmAdvanceLogin.frx":0CCA
      Left            =   2160
      List            =   "frmAdvanceLogin.frx":0CD7
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   3
      Top             =   60
      Width           =   1815
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2760
      TabIndex        =   1
      Top             =   4800
      Width           =   1695
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   480
      TabIndex        =   0
      Top             =   4800
      Width           =   1575
   End
   Begin VB.Line Line1 
      X1              =   240
      X2              =   4920
      Y1              =   3360
      Y2              =   3360
   End
   Begin VB.Label Label8 
      Caption         =   "-"
      Height          =   255
      Left            =   3000
      TabIndex        =   20
      Top             =   4080
      Width           =   255
   End
   Begin VB.Label Label7 
      Caption         =   "RTP Port Range:"
      Height          =   255
      Left            =   720
      TabIndex        =   17
      Top             =   4080
      Width           =   1335
   End
   Begin VB.Label Label6 
      Caption         =   "Local VoIP Port:"
      Height          =   255
      Left            =   840
      TabIndex        =   16
      Top             =   3630
      Width           =   1215
   End
   Begin VB.Label Label5 
      Caption         =   "Application Root:"
      Height          =   255
      Left            =   840
      TabIndex        =   9
      Top             =   1680
      Width           =   1215
   End
   Begin VB.Label Label4 
      Caption         =   "Realtime Port"
      Height          =   255
      Left            =   840
      TabIndex        =   8
      Top             =   2760
      Width           =   975
   End
   Begin VB.Label Label3 
      Caption         =   "Realtime URL:"
      Height          =   255
      Left            =   840
      TabIndex        =   7
      Top             =   2220
      Width           =   1095
   End
   Begin VB.Label Label2 
      Caption         =   "Socket Port:"
      Height          =   255
      Left            =   840
      TabIndex        =   6
      Top             =   1140
      Width           =   1095
   End
   Begin VB.Label Label1 
      Caption         =   "Socket URL:"
      Height          =   255
      Left            =   840
      TabIndex        =   5
      Top             =   600
      Width           =   1095
   End
   Begin VB.Line Line3 
      X1              =   120
      X2              =   4800
      Y1              =   480
      Y2              =   480
   End
   Begin VB.Label Label10 
      Caption         =   "Set To Defaults For:"
      Height          =   255
      Left            =   600
      TabIndex        =   2
      Top             =   120
      Width           =   1935
   End
End
Attribute VB_Name = "frmAdvanceLogin"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub cmdCancel_Click()
Me.Hide
End Sub



Private Sub cmdOK_Click()
  SaveKey "SocketURL", txtSocketURL
  SaveKey "SocketPort", txtSocketPort
  SaveKey "AppRoot", txtAppRoot
  SaveKey "RealtimeURL", txtRealtimeURL
  SaveKey "RealtimePort", txtRealtimePort
  
    
  Me.Hide
End Sub


Private Sub Combo1_Change()

End Sub

Private Sub cmdReset_Click()
log_it cmbPresets.List(cmbPresets.ListIndex)
log_it "CHANGE"
Select Case cmbPresets.List(cmbPresets.ListIndex)
Case "EQ Trainer"
    EQTrainerPresets
Case "VeeCue"
    VeeCuePresets
Case "BC"
    BCPresets
End Select

End Sub

Private Sub EQTrainerPresets()
 With frmAdvanceLogin
    .txtSocketURL = "voip.idapted.com"
    .txtSocketPort = 7000
    .txtAppRoot = "www.idapted.com"
    .txtRealtimePort = 7000
    .txtRealtimeURL = "realtime.idapted.com"
  
   End With
End Sub


Private Sub BCPresets()
 With frmAdvanceLogin
    .txtSocketURL = "voip.englishonlinevoip.com"
    .txtSocketPort = 6999
    .txtAppRoot = "www.englishonlinevoip.com"
    .txtRealtimePort = 7000
    .txtRealtimeURL = "realtime.idapted.com"
  
   End With
End Sub

Private Sub VeeCuePresets()
 With frmAdvanceLogin
    .txtSocketURL = "192.168.1.30"
    .txtSocketPort = 7000
    .txtAppRoot = "w.veecue.com"
    .txtRealtimePort = 7000
    .txtRealtimeURL = "realtime.veecue.com"
   End With
End Sub

