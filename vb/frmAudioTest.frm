VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "comctl32.ocx"
Begin VB.Form frmAudioTest 
   Caption         =   "Audio Test"
   ClientHeight    =   2820
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4560
   Icon            =   "frmAudioTest.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   2820
   ScaleWidth      =   4560
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame frmMicTest 
      BorderStyle     =   0  'None
      Height          =   2055
      Left            =   120
      TabIndex        =   23
      Top             =   480
      Visible         =   0   'False
      Width           =   4215
      Begin VB.Frame frmMicDisplay 
         Caption         =   "Microphone"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1110
         Left            =   0
         TabIndex        =   24
         Top             =   960
         Width           =   4035
         Begin ComctlLib.ProgressBar MicProgress 
            Height          =   315
            Left            =   885
            TabIndex        =   25
            Top             =   285
            Width           =   2925
            _ExtentX        =   5159
            _ExtentY        =   556
            _Version        =   327682
            Appearance      =   1
            Max             =   9
         End
         Begin ComctlLib.Slider SliderMicVol 
            Height          =   255
            Left            =   840
            TabIndex        =   26
            Top             =   645
            Width           =   2955
            _ExtentX        =   5212
            _ExtentY        =   450
            _Version        =   327682
         End
         Begin VB.Label lblMicVolume 
            Caption         =   "Volume"
            BeginProperty Font 
               Name            =   "Arial"
               Size            =   9
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   1
            Left            =   195
            TabIndex        =   28
            Top             =   630
            Width           =   660
         End
         Begin VB.Label lblMicLevel 
            Caption         =   "Level"
            BeginProperty Font 
               Name            =   "Arial"
               Size            =   9
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   240
            Index           =   1
            Left            =   300
            TabIndex        =   27
            Top             =   285
            Width           =   480
         End
      End
      Begin VB.Label lblMicTest 
         Alignment       =   2  'Center
         Caption         =   "Connecting..."
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   615
         Left            =   120
         TabIndex        =   29
         Top             =   0
         Width           =   3855
      End
   End
   Begin VB.Frame frmSpeakerError 
      Height          =   1500
      Left            =   360
      TabIndex        =   19
      Top             =   600
      Visible         =   0   'False
      Width           =   3525
      Begin VB.CommandButton cmdTroubleshoot 
         Caption         =   "Troubleshoot"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   345
         Index           =   1
         Left            =   660
         TabIndex        =   22
         Top             =   1050
         Width           =   2235
      End
      Begin VB.CommandButton cmdRetrySpeaker 
         Caption         =   "Retry Speaker Test"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Left            =   510
         TabIndex        =   20
         Top             =   555
         Width           =   2565
      End
      Begin VB.Label Label8 
         Alignment       =   2  'Center
         Caption         =   "What would you like to do?"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   600
         TabIndex        =   21
         Top             =   210
         Width           =   2340
      End
   End
   Begin VB.Frame frmFinished 
      Height          =   1845
      Left            =   360
      TabIndex        =   15
      Top             =   600
      Visible         =   0   'False
      Width           =   3975
      Begin VB.CommandButton cmdNoHear 
         Caption         =   "NO, I could not hear the recording clearly."
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   510
         Left            =   480
         TabIndex        =   18
         Top             =   705
         Width           =   3120
      End
      Begin VB.CommandButton cmdSpeakerOK 
         Caption         =   "Yes, I heard that clearly."
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   300
         Left            =   503
         TabIndex        =   17
         Top             =   1395
         Width           =   3075
      End
      Begin VB.Label Label7 
         Caption         =   "Could you hear yourself clearly?"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   330
         Left            =   683
         TabIndex        =   16
         Top             =   270
         Width           =   2715
      End
   End
   Begin VB.Frame frmSpkTest 
      BorderStyle     =   0  'None
      Height          =   2205
      Left            =   0
      TabIndex        =   9
      Top             =   480
      Visible         =   0   'False
      Width           =   4140
      Begin VB.Frame frmSpkDisplay 
         Caption         =   "Headphones"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1005
         Left            =   120
         TabIndex        =   10
         Top             =   1200
         Width           =   4020
         Begin ComctlLib.ProgressBar SpkProgress 
            Height          =   300
            Left            =   930
            TabIndex        =   11
            Top             =   240
            Width           =   2910
            _ExtentX        =   5133
            _ExtentY        =   529
            _Version        =   327682
            Appearance      =   1
            Max             =   9
         End
         Begin ComctlLib.Slider SliderSpkVol 
            Height          =   255
            Left            =   915
            TabIndex        =   12
            Top             =   615
            Width           =   2910
            _ExtentX        =   5133
            _ExtentY        =   450
            _Version        =   327682
         End
         Begin VB.Label Label1 
            Caption         =   "Level"
            BeginProperty Font 
               Name            =   "Arial"
               Size            =   9
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   240
            Index           =   0
            Left            =   375
            TabIndex        =   14
            Top             =   300
            Width           =   480
         End
         Begin VB.Label Label2 
            Caption         =   "Volume"
            BeginProperty Font 
               Name            =   "Arial"
               Size            =   9
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   225
            Index           =   0
            Left            =   270
            TabIndex        =   13
            Top             =   645
            Width           =   660
         End
      End
      Begin VB.Label lblSpkTest 
         Alignment       =   2  'Center
         Caption         =   "Connecting..."
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   615
         Left            =   240
         TabIndex        =   30
         Top             =   120
         Width           =   3855
      End
   End
   Begin VB.Frame frmMicError 
      Height          =   2070
      Left            =   120
      TabIndex        =   7
      Top             =   480
      Visible         =   0   'False
      Width           =   4215
      Begin VB.CommandButton cmdRetry 
         Caption         =   "Retry"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   345
         Left            =   1065
         TabIndex        =   32
         Top             =   975
         Width           =   2235
      End
      Begin VB.CommandButton cmdTroubleshoot 
         Caption         =   "Troubleshoot"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   345
         Index           =   0
         Left            =   1080
         TabIndex        =   31
         Top             =   1455
         Width           =   2235
      End
      Begin VB.Label Label6 
         Caption         =   "Oops! Your voice was NOT heard by the system.  Double check your headset is properly plugged in and either:"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   615
         Left            =   165
         TabIndex        =   8
         Top             =   240
         Width           =   3975
      End
   End
   Begin VB.Frame frmContinue 
      Height          =   1815
      Left            =   120
      TabIndex        =   4
      Top             =   480
      Visible         =   0   'False
      Width           =   4245
      Begin VB.CommandButton cmdContinue 
         Caption         =   "Continue to Headphone Test"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Left            =   720
         TabIndex        =   6
         Top             =   1290
         Width           =   2880
      End
      Begin VB.Label Label5 
         Caption         =   $"frmAudioTest.frx":0CCA
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   900
         Left            =   165
         TabIndex        =   5
         Top             =   270
         Width           =   3915
      End
   End
   Begin VB.Frame frmBegin 
      Height          =   2265
      Left            =   120
      TabIndex        =   1
      Top             =   480
      Width           =   4200
      Begin VB.CommandButton cmdSkipTest 
         Caption         =   "Skip Echo Test"
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
         Left            =   840
         TabIndex        =   33
         Top             =   1800
         Visible         =   0   'False
         Width           =   2535
      End
      Begin VB.CommandButton cmdBegin 
         Caption         =   "Begin Mic Test"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   345
         Left            =   825
         TabIndex        =   3
         Top             =   1215
         Width           =   2610
      End
      Begin VB.Label Label4 
         Caption         =   "We will now test your microphone. Press Begin and after the beep, say ""Testing one, two, three"" outloud."
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   855
         Left            =   240
         TabIndex        =   2
         Top             =   195
         Width           =   3690
      End
   End
   Begin VB.Label Label3 
      Alignment       =   2  'Center
      Caption         =   "Echo Test"
      BeginProperty Font 
         Name            =   "Arial Black"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   1560
      TabIndex        =   0
      Top             =   60
      Width           =   1545
   End
End
Attribute VB_Name = "frmAudioTest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub ExitEchoTest()
 PreLoadFlashFiles
  ToggleAvailability False
Me.Hide
End Sub

Private Sub cmdSkipTest_Click()
ExitEchoTest
End Sub

Private Sub SliderMicVol_Click()
    ' SetMicVolume method requires Volume value from 0 to 255 range    '
    ' but the slider range is 0 to 10, so we multiply the slider value '
    ' to 25 to get the required value to pass to SetMicVolume method   '
    
    frmMain.oVoIP.SetMixerVolume 9, SliderMicVol.value * 10
End Sub

Private Sub SliderSpkVol_Click()

' SetSpkVolume method requires Volume value from 0 to 255 range    '
' but the slider range is 0 to 10, so we multiply the slider value '
' to 25 to get the required value to pass to SetSpkVolume method   '
    
    

    frmMain.oVoIP.SetMixerVolume 8, SliderSpkVol.value * 10
    
End Sub
Private Sub Timer1_Timer()

End Sub

Private Sub cmdBegin_Click()
  BeginEchoTest
End Sub

Private Sub cmdContinue_Click()
BeginSpeakerTest
End Sub

Private Sub cmdNoHear_Click()
frmSpeakerError.Visible = True
frmFinished.Visible = False
End Sub

Private Sub cmdRetry_Click()
frmMicError.Visible = False
frmBegin.Visible = True
End Sub

Private Sub cmdRetrySpeaker_Click()
BeginSpeakerTest
End Sub

Private Sub cmdSpeakerOK_Click()

  ExitEchoTest
End Sub

Private Sub cmdTroubleshoot_Click(index As Integer)
frmTroubleshoot.Show vbModal
End Sub

Private Sub Form_Load()
blnDetectLevel = False
End Sub

Private Sub Form_Unload(Cancel As Integer)

If blnExitOnCloseTest Then
  UnloadAll
End If
End Sub

Private Sub tmrLevels_Timer()

End Sub
