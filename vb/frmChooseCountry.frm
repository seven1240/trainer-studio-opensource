VERSION 5.00
Begin VB.Form frmChooseCountry 
   BorderStyle     =   5  'Sizable ToolWindow
   Caption         =   "Choose Region"
   ClientHeight    =   2805
   ClientLeft      =   60
   ClientTop       =   300
   ClientWidth     =   4785
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2805
   ScaleWidth      =   4785
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton Command2 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2520
      TabIndex        =   6
      Top             =   2160
      Width           =   855
   End
   Begin VB.ComboBox cmbProduct 
      Height          =   315
      ItemData        =   "frmChooseCountry.frx":0000
      Left            =   1200
      List            =   "frmChooseCountry.frx":000D
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   960
      Width           =   3135
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Continue"
      Default         =   -1  'True
      Height          =   375
      Left            =   3600
      TabIndex        =   2
      Top             =   2160
      Width           =   975
   End
   Begin VB.ComboBox lstRegion 
      Height          =   315
      ItemData        =   "frmChooseCountry.frx":004F
      Left            =   1200
      List            =   "frmChooseCountry.frx":0065
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   1
      Top             =   1560
      Width           =   3135
   End
   Begin VB.Label Label2 
      Caption         =   "I  work for:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   240
      TabIndex        =   5
      Top             =   960
      Width           =   975
   End
   Begin VB.Label Label3 
      Caption         =   "I am in:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   480
      TabIndex        =   4
      Top             =   1560
      Width           =   615
   End
   Begin VB.Label Label1 
      Caption         =   "Welcome! To complete the installation this program needs to know a few things about you."
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   240
      TabIndex        =   3
      Top             =   120
      Width           =   4215
   End
End
Attribute VB_Name = "frmChooseCountry"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()
'Me.Tag = lstRegion.List(lstRegion.ListIndex)
Me.Hide
End Sub

Private Sub Command2_Click()
End
End Sub

