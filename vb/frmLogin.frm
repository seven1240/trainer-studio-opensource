VERSION 5.00
Begin VB.Form frmLogin 
   Appearance      =   0  'Flat
   AutoRedraw      =   -1  'True
   BackColor       =   &H80000004&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Login"
   ClientHeight    =   3015
   ClientLeft      =   8340
   ClientTop       =   6345
   ClientWidth     =   4725
   Icon            =   "frmLogin.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3015
   ScaleWidth      =   4725
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame frmStatus 
      Caption         =   "Logging In..."
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2775
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Visible         =   0   'False
      Width           =   4455
      Begin VB.Label lblStatus 
         BeginProperty Font 
            Name            =   "Arial Black"
            Size            =   14.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1005
         Left            =   360
         TabIndex        =   6
         Top             =   600
         Width           =   3690
      End
   End
   Begin VB.Frame fraLogin 
      Caption         =   "Please Login"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2775
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4455
      Begin VB.CommandButton cmdAdvanced 
         Caption         =   "Advanced"
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
         Left            =   3120
         TabIndex        =   7
         Top             =   1800
         Width           =   975
      End
      Begin VB.TextBox txtUsername 
         Height          =   285
         Left            =   1080
         TabIndex        =   1
         Top             =   960
         Width           =   3015
      End
      Begin VB.TextBox txtPassword 
         Height          =   285
         IMEMode         =   3  'DISABLE
         Left            =   1080
         PasswordChar    =   "*"
         TabIndex        =   2
         Top             =   1440
         Width           =   3015
      End
      Begin VB.CommandButton cmdLogin 
         Caption         =   "Login"
         Default         =   -1  'True
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
         Left            =   1560
         TabIndex        =   3
         Top             =   2160
         Width           =   1215
      End
      Begin VB.Label Label2 
         Caption         =   "Password"
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
         Left            =   120
         TabIndex        =   9
         Top             =   1560
         Width           =   855
      End
      Begin VB.Label Label1 
         Caption         =   "Username"
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
         Left            =   120
         TabIndex        =   8
         Top             =   1080
         Width           =   975
      End
      Begin VB.Label lblError 
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H000000FF&
         Height          =   615
         Left            =   120
         TabIndex        =   4
         Top             =   240
         Width           =   4095
      End
   End
End
Attribute VB_Name = "frmLogin"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False



Private Sub cmdAdvanced_Click()
LoadfrmAdvanceSettings
frmAdvanceLogin.Show vbModal

End Sub

Private Sub cmdLogin_Click()
Dim strResult As String

SaveKey "Username", txtUsername

Screen.MousePointer = 11
frmStatus.Visible = True
fraLogin.Visible = False
SetLoginStatus "Checking for client updates..."

ConnectToSocket

End Sub

Private Sub ConnectToSocket()
    frmMain.Sock.RemoteHost = frmAdvanceLogin.txtSocketURL
    frmMain.Sock.RemotePort = CInt(frmAdvanceLogin.txtSocketPort)
    frmMain.Sock.Tag = "Connect"
    frmMain.Sock.Connect
End Sub

Private Sub Form_Activate()
If Not txtUsername.Text = "" And Me.Visible = True And fraLogin.Visible = True Then
    txtPassword.SetFocus
End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
UnloadAll
End Sub


