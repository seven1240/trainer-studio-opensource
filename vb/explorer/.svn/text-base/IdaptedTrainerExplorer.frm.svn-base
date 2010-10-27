VERSION 5.00
Object = "{EAB22AC0-30C1-11CF-A7EB-0000C05BAE0B}#1.1#0"; "shdocvw.dll"
Begin VB.Form frmBrowser 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Idapted Interaction Screen"
   ClientHeight    =   10020
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   13050
   ClipControls    =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   668
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   870
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdReload 
      Caption         =   "Reload"
      Height          =   240
      Left            =   30
      TabIndex        =   9
      Top             =   9750
      Width           =   945
   End
   Begin VB.CommandButton cmdRecord 
      Caption         =   "Record"
      Height          =   255
      Left            =   11760
      TabIndex        =   7
      Top             =   9720
      Width           =   1095
   End
   Begin VB.Frame Frame1 
      Height          =   615
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Width           =   12855
      Begin VB.Timer Timer1 
         Enabled         =   0   'False
         Interval        =   200
         Left            =   6240
         Tag             =   "0"
         Top             =   120
      End
      Begin VB.CommandButton cmdHangup 
         Caption         =   "End Call"
         Height          =   375
         Left            =   11040
         TabIndex        =   1
         Top             =   150
         Width           =   1575
      End
      Begin VB.Label lblReview 
         Caption         =   "Save the Review Screen to Close this Window"
         Height          =   255
         Left            =   9000
         TabIndex        =   6
         Top             =   240
         Visible         =   0   'False
         Width           =   3735
      End
      Begin VB.Label lblLoading 
         Caption         =   "Loading..."
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   255
         Left            =   120
         TabIndex        =   5
         Top             =   240
         Width           =   3495
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
         Height          =   330
         Left            =   8565
         TabIndex        =   2
         Top             =   195
         Width           =   2355
      End
   End
   Begin SHDocVwCtl.WebBrowser Browser 
      Height          =   9000
      Left            =   0
      TabIndex        =   3
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
   Begin VB.Label lblRecord 
      Caption         =   "Recording..."
      Height          =   270
      Left            =   11640
      TabIndex        =   8
      Top             =   9735
      Visible         =   0   'False
      Width           =   1215
   End
   Begin VB.Label lblStatus 
      Height          =   255
      Left            =   1110
      TabIndex        =   4
      Top             =   9720
      Width           =   11745
   End
End
Attribute VB_Name = "frmBrowser"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Finished As Boolean
Private Sub DoInitialNavigation()
 Dim strArgs() As String
 Dim vPostData As Variant
 Dim vHeaders As Variant
 Dim strContentHeader As String
 Dim strBody As String
 Dim aPostData() As Byte
 
 'assemble the body. send one field and one file
 strBody = "username=" + Username + "&password=" & Password + "&display_name=" & CallerID
 'must convert to byte array
 strContentHeader = "Content-Type: application/x-www-form-urlencoded" & vbCrLf
 aPostData = StrConv(strBody, vbFromUnicode)
 vPostData = aPostData

 frmBrowser.Browser.Navigate BaseURL + "/get_interaction", 0, "", vPostData, vHeaders

End Sub
Private Sub startLoading()
 Timer1.Tag = 0
 Timer1.Enabled = True
End Sub

Private Sub stopLoading()
 Timer1.Enabled = False
 lblLoading = ""
End Sub

Private Sub Browser_BeforeNavigate2(ByVal pDisp As Object, URL As Variant, Flags As Variant, TargetFrameName As Variant, PostData As Variant, Headers As Variant, Cancel As Boolean)
 startLoading
End Sub


Private Sub Browser_NavigateComplete2(ByVal pDisp As Object, URL As Variant)
Dim pos As Long, strFile As String
 On Error GoTo err
stopLoading
If InStr(1, URL, "trainer_finished") Then
 Finished = True
 Unload Me
ElseIf InStr(1, URL, "#PLAYSOUND") Then
 pos = InStr(1, URL, "#PLAYSOUND")

 strFile = Right(URL, Len(URL) - pos - 10)
  
 SendMsg CLIENT, WM_IDP_PLAYSOUND, CLng(strFile)
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
Exit Sub
err:
MsgBox "Error Occured in Navigate.  Probably invalid sound filename"
End Sub

Private Sub Browser_NavigateError(ByVal pDisp As Object, URL As Variant, Frame As Variant, StatusCode As Variant, Cancel As Boolean)
  Clipboard.SetText "URL: " + CStr(URL) + " StatusCode: " + CStr(StatusCode) + " Frame: " + CStr(Frame) + " Cancel: " + CStr(Cancel)
  MsgBox "Navigation Error.  The relevant error information has been automatically copied to your clipboard.  Please paste this information (ctrl+v) into the trainer chat to help Jonathan debug!  Thanks. (Error code " + CStr(StatusCode) + ")"
End Sub

Private Sub Browser_StatusTextChange(ByVal Text As String)

lblStatus.Caption = Text
End Sub

Private Sub cmdHangup_Click()
 Disconnect
End Sub

Private Sub cmdRecord_Click()
  SendMsg CLIENT, WM_IDP_RECORD
  cmdRecord.Visible = False
  lblRecord.Visible = True
End Sub

Private Sub cmdReload_Click()
DoInitialNavigation
End Sub

Private Sub Form_Load()
 Dim strArgs() As String
 Dim vPostData As Variant
 Dim vHeaders As Variant
 Dim strContentHeader As String
 Dim strBody As String
 Dim aPostData() As Byte

 Finished = False
 ''''''''''''''''''''
 ' Setup Subclassing
 ''''''''''''''''''
 SetWindowProc (Me.hwnd)

 ''''''''''''''''''''''''
 ' Split the arguement and go to the URL
 ''''''''''''''''''''''''''''''''
 strArgs = Split(Command, " ")
 Username = strArgs(0)
 Password = strArgs(1)
 CallerID = strArgs(2)
 BaseURL = strArgs(3)

DoInitialNavigation
End Sub

Private Sub Form_Unload(Cancel As Integer)
 If Finished = False Then
  Dim re As Long
  re = MsgBox("Closing this window will end the interaction and you will not be compensated for it.  Are you sure you want to continue?", vbYesNo)
  If re = vbYes Then
   Unload Me
  Else
   Cancel = True
  End If
 End If
End Sub

Private Sub Timer1_Timer()
 Timer1.Tag = Timer1.Tag + 1
 If Timer1.Tag > 5 Then Timer1.Tag = 0

  lblLoading.Caption = "Loading Page " + String(Timer1.Tag, ".")
End Sub
