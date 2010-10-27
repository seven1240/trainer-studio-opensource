VERSION 5.00
Object = "{EAB22AC0-30C1-11CF-A7EB-0000C05BAE0B}#1.1#0"; "shdocvw.dll"
Begin VB.Form frmBrowser 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Idapted Interaction"
   ClientHeight    =   9795
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   13080
   Icon            =   "frmBrowser.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   653
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   872
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame Frame1 
      Height          =   615
      Left            =   120
      TabIndex        =   1
      Top             =   0
      Width           =   12855
      Begin VB.CommandButton cmdHangup 
         Caption         =   "End Call"
         Height          =   375
         Left            =   11040
         TabIndex        =   2
         Top             =   120
         Width           =   1575
      End
      Begin VB.Label lblTime 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   8520
         TabIndex        =   4
         Top             =   240
         Width           =   2055
      End
      Begin VB.Label lblStatus 
         Height          =   255
         Left            =   120
         TabIndex        =   3
         Top             =   240
         Width           =   8295
      End
   End
   Begin SHDocVwCtl.WebBrowser Browser 
      Height          =   9000
      Left            =   120
      TabIndex        =   0
      Top             =   720
      Width           =   12915
      ExtentX         =   22781
      ExtentY         =   15875
      ViewMode        =   0
      Offline         =   0
      Silent          =   0
      RegisterAsBrowser=   0
      RegisterAsDropTarget=   1
      AutoArrange     =   0   'False
      NoClientEdge    =   0   'False
      AlignLeft       =   0   'False
      NoWebView       =   0   'False
      HideFileNames   =   0   'False
      SingleClick     =   0   'False
      SingleSelection =   0   'False
      NoFolders       =   0   'False
      Transparent     =   0   'False
      ViewID          =   "{0057D0E0-3573-11CF-AE69-08002B2E1262}"
      Location        =   "http:///"
   End
End
Attribute VB_Name = "frmBrowser"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Browser_BeforeNavigate2(ByVal pDisp As Object, URL As Variant, Flags As Variant, TargetFrameName As Variant, PostData As Variant, Headers As Variant, Cancel As Boolean)
If Not (InStr(1, URL, "interaction") > 1) Then
 Cancel = True
End If
End Sub

Private Sub Browser_NavigateComplete2(ByVal pDisp As Object, URL As Variant)
Dim pos As Long, strFile As String
If InStr(1, URL, "trainer_finished") Then
    End
'ElseIf InStr(1, URL, "#PLAYSOUND") Then
'   pos = InStr(1, URL, "#PLAYSOUND")
'
'   strFile = Right(URL, Len(URL) - pos - 9)
'  Dim error As Long
'  error = frmMain.VaxSIPUserAgentOCX.PlayWaveOpen(App.Path + "\sounds\" + strFile + ".wav")
'If error = 0 Then
'    ErrorMessages
'End If
'error = frmMain.VaxSIPUserAgentOCX.PlayWaveStart(True)
'If error = 0 Then
'   ErrorMessages
'End If
End If
End Sub



Private Sub cmdHangup_Click()
EndInteraction
End Sub

Private Sub Command1_Click()
Dim error As Long
MsgBox App.Path + "\sounds\L10001.wav"
error = frmMain.VaxSIPUserAgentOCX.PlayWaveOpen(App.Path + "\sounds\L10001.wav")
If error <> 0 Then
    MsgBox "error" 'error'
    ErrorMessages
End If
error = frmMain.VaxSIPUserAgentOCX.PlayWaveStart(True)
MsgBox error
If error <> 0 Then
   ErrorMessages
End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
     Dim re As Long
        re = MsgBox("Closing this window will end the interaction and you will not be compensated for it.  Are you sure you want to continue?", vbYesNo)
        If re = vbYes Then
            EndInteractionHideForms
        Else
            Cancel = True
        End If
End Sub

