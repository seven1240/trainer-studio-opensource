VERSION 5.00
Begin VB.Form frmDialer 
   Caption         =   "Trainer Phonebook"
   ClientHeight    =   4455
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4275
   Icon            =   "frmDialer.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   4455
   ScaleWidth      =   4275
   StartUpPosition =   3  '´°¿ÚÈ±Ê¡
   Begin VB.CommandButton cmdRemoveNumber 
      Caption         =   "Remove Number"
      Height          =   375
      Left            =   2280
      TabIndex        =   4
      Top             =   3840
      Width           =   1695
   End
   Begin VB.CommandButton cmdAdd 
      Caption         =   "Add Number"
      Height          =   375
      Left            =   120
      TabIndex        =   3
      Top             =   3840
      Width           =   1815
   End
   Begin VB.ListBox lstNumbers 
      Height          =   2760
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   3975
   End
   Begin VB.CommandButton cmdCall 
      Caption         =   "Call"
      Height          =   375
      Left            =   3120
      TabIndex        =   1
      Top             =   240
      Width           =   975
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Left            =   120
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   240
      Width           =   2895
   End
End
Attribute VB_Name = "frmDialer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
