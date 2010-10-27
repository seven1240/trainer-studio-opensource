VERSION 5.00
Begin VB.Form frmTroubleshoot 
   Caption         =   "Troubleshoot Audio"
   ClientHeight    =   3660
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3660
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2760
      TabIndex        =   10
      Top             =   3240
      Width           =   1335
   End
   Begin VB.CommandButton cmdClose 
      Caption         =   "Save"
      Height          =   345
      Left            =   480
      TabIndex        =   2
      Top             =   3240
      Width           =   1365
   End
   Begin VB.Frame Frame2 
      Caption         =   "Troubleshoot Steps"
      Height          =   1605
      Left            =   120
      TabIndex        =   1
      Top             =   45
      Width           =   4395
      Begin VB.Label Label3 
         Caption         =   "3.  Are we using the correct sound device?"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   285
         Left            =   105
         TabIndex        =   5
         Top             =   1200
         Width           =   4140
      End
      Begin VB.Label Label2 
         Caption         =   "2. Verify the system volume is turned up and unmuted."
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   555
         Left            =   90
         TabIndex        =   4
         Top             =   720
         Width           =   3960
      End
      Begin VB.Label Label1 
         Caption         =   "1. Double check that you headset is correctly plugged in."
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   495
         Left            =   90
         TabIndex        =   3
         Top             =   240
         Width           =   4170
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Choose Sound Device"
      Height          =   1275
      Left            =   120
      TabIndex        =   0
      Top             =   1800
      Width           =   4410
      Begin VB.ComboBox ListAudioIn 
         Height          =   315
         Left            =   1005
         Style           =   2  'Dropdown List
         TabIndex        =   7
         Top             =   360
         Width           =   3210
      End
      Begin VB.ComboBox ListAudioOut 
         Height          =   300
         Left            =   960
         TabIndex        =   6
         Top             =   840
         Width           =   3240
      End
      Begin VB.Label Label5 
         Caption         =   "Headphone"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   7.5
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   225
         Left            =   60
         TabIndex        =   9
         Top             =   840
         Width           =   870
      End
      Begin VB.Label Label4 
         Caption         =   "Mic:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   6.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   525
         TabIndex        =   8
         Top             =   435
         Width           =   360
      End
   End
End
Attribute VB_Name = "frmTroubleshoot"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Sub UpdateAudioIn()
    On Error GoTo err
    Dim DeviceId As Long
    ListAudioIn.Clear
    ListAudioIn.AddItem ("Default Device")
    
    For DeviceId = 0 To frmMain.oVoIP.GetInDeviceCount() - 1
        ListAudioIn.AddItem (frmMain.oVoIP.GetInDeviceName(DeviceId))
    Next DeviceId
    
    ListAudioIn.ListIndex = AudioInDev
    
    Exit Sub
err:
    ListAudioIn.ListIndex = 0
    
    
End Sub

Private Sub UpdateAudioOut()
    On Error GoTo err
    Dim DeviceId As Long
    ListAudioOut.Clear
    ListAudioOut.AddItem ("Default Device")
    
    For DeviceId = 0 To frmMain.oVoIP.GetOutDeviceCount() - 1
        ListAudioOut.AddItem (frmMain.oVoIP.GetOutDeviceName(DeviceId))
    Next DeviceId
    
    ListAudioOut.ListIndex = AudioOutDev
    
    Exit Sub
err:
    ListAudioOut.ListIndex = 0
End Sub
Private Sub cmdClose_Click()
Dim Re As VbMsgBoxResult

Re = MsgBox("Changing Speaker or Audio Device requires that you re-login.  Continue?", vbQuestion Or vbYesNo)

If Re = vbYes Then

SaveKey "AudioInDev", ListAudioIn.ListIndex
SaveKey "AudioOutDev", ListAudioOut.ListIndex

AudioInDev = ListAudioIn.ListIndex
AudioOutDev = ListAudioOut.ListIndex

Me.Hide
frmAudioTest.Hide
frmLogin.Show
Login

End If

End Sub

Private Sub Form_Load()
UpdateAudioIn
UpdateAudioOut
End Sub

