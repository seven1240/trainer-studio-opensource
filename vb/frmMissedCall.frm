VERSION 5.00
Begin VB.Form frmForcedPause 
   Caption         =   "Missed Call"
   ClientHeight    =   2775
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5535
   ControlBox      =   0   'False
   Icon            =   "frmMissedCall.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   2775
   ScaleWidth      =   5535
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton cmdPaused 
      Caption         =   "I'd Like to Stay Paused"
      Default         =   -1  'True
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   240
      TabIndex        =   2
      Top             =   2040
      Width           =   5055
   End
   Begin VB.CommandButton cmdStartWorking 
      Caption         =   "I'd Like to Start Working Again"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   240
      TabIndex        =   1
      Top             =   1320
      Width           =   5055
   End
   Begin VB.Label lblReason 
      Alignment       =   2  'Center
      Caption         =   "You missed a call.  Can you please respond in the chat room"
      BeginProperty Font 
         Name            =   "Arial Black"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   120
      TabIndex        =   3
      Top             =   600
      Width           =   5175
   End
   Begin VB.Label lblCaption 
      Alignment       =   2  'Center
      Caption         =   "You have been automatically paused because:"
      BeginProperty Font 
         Name            =   "Arial Black"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   855
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   5175
   End
End
Attribute VB_Name = "frmForcedPause"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


Private Sub cmdPaused_Click()
Me.Hide
frmMain.SetFocus
End Sub

Private Sub cmdStartWorking_Click()
ToggleAvailability True
Me.Hide
frmMain.SetFocus
End Sub

Private Sub Form_Load()
Call SetWindowPos(Me.hwnd, HWND_TOPMOST, 0, 0, 0, 0, FLAGS)
End Sub
