VERSION 5.00
Begin VB.Form frmIncoming 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Incoming Call..."
   ClientHeight    =   630
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5895
   Icon            =   "frmIncoming.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   630
   ScaleWidth      =   5895
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton cmdReject 
      Cancel          =   -1  'True
      Caption         =   "Reject"
      Height          =   375
      Left            =   3240
      TabIndex        =   1
      Top             =   120
      Width           =   2415
   End
   Begin VB.CommandButton cmdAccept 
      Caption         =   "Accept"
      Default         =   -1  'True
      Height          =   375
      Left            =   360
      TabIndex        =   0
      Top             =   120
      Width           =   2415
   End
End
Attribute VB_Name = "frmIncoming"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Copyright ©1996-2007 VBnet, Randy Birch, All Rights Reserved.
' Some pages may also contain other copyrights by the author.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Distribution: You can freely use this code in your own
'               applications, but you may not reproduce
'               or publish this code on any web site,
'               online service, or distribute as source
'               on any media without express permission.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''




Private Sub cmdAccept_Click()

log_it "Answering call"
frmMain.oVoIP.AnswerCall



End Sub

Private Sub cmdReject_Click()
 On Error Resume Next
StopRing
frmMain.oVoIP.RejectInboundCall
Me.Hide

End Sub

Private Sub Form_Activate()

' Me.SetFocus

End Sub

Private Sub Form_Load()
 Call SetWindowPos(Me.hwnd, HWND_TOPMOST, 0, 0, 0, 0, FLAGS)
End Sub

Private Sub Form_Unload(Cancel As Integer)
cmdReject_Click
End Sub
