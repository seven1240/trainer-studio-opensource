VERSION 5.00
Begin VB.Form frmSettings 
   BorderStyle     =   3  'Fixed Dialog
   ClientHeight    =   2520
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   2760
   Icon            =   "frmSettings.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2520
   ScaleWidth      =   2760
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdEcho 
      Caption         =   "Echo Test"
      Height          =   495
      Left            =   480
      TabIndex        =   5
      Top             =   1920
      Width           =   1575
   End
   Begin VB.Frame Frame8 
      Caption         =   " Volume Settings "
      Height          =   1695
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2535
      Begin VB.CheckBox CheckMuteMic 
         Caption         =   "Mute"
         Height          =   255
         Left            =   1680
         TabIndex        =   2
         Top             =   960
         Width           =   735
      End
      Begin VB.CheckBox CheckSpkMute 
         Caption         =   "Mute"
         Height          =   255
         Left            =   1680
         TabIndex        =   1
         Top             =   240
         Width           =   735
      End
      Begin VB.Label LabelMicVol 
         Caption         =   "Microphone:"
         Height          =   255
         Left            =   120
         TabIndex        =   4
         Top             =   960
         Width           =   1215
      End
      Begin VB.Label LabelSpeakerVol 
         Caption         =   "Speaker:"
         Height          =   255
         Left            =   120
         TabIndex        =   3
         Top             =   240
         Width           =   855
      End
   End
End
Attribute VB_Name = "frmSettings"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub CheckMuteMic_Click()
    Dim Result
    Result = frmMain.VaxSIPUserAgentOCX.MuteMic(CheckMuteMic.value)
End Sub

Private Sub CheckSpkMute_Click()
    Dim Result
    Result = frmMain.VaxSIPUserAgentOCX.MuteSpk(CheckSpkMute.value)
End Sub


Private Sub SliderMicVol_Click()
    ' SetMicVolume method requires Volume value from 0 to 255 range    '
    ' but the slider range is 0 to 10, so we multiply the slider value '
    ' to 25 to get the required value to pass to SetMicVolume method   '
    
    Dim Result
    Result = frmMain.VaxSIPUserAgentOCX.SetMicVolume(SliderMicVol.value * 25)
        
    If Result = False Then
        ErrorMessages
    End If
End Sub

Private Sub SliderSpkVol_Click()

' SetSpkVolume method requires Volume value from 0 to 255 range    '
' but the slider range is 0 to 10, so we multiply the slider value '
' to 25 to get the required value to pass to SetSpkVolume method   '
    
    Dim Result
    Result = frmMain.VaxSIPUserAgentOCX.PlayWaveOpen(LINENUM, App.Path + "/sounds/L10003.wav")
    If Result = False Then
        MsgBox "here"
        ErrorMessages
    End If
    frmMain.VaxSIPUserAgentOCX.PlayWaveStart LINENUM, True
    Result = frmMain.VaxSIPUserAgentOCX.SetSpkVolume(SliderSpkVol.value * 25)
    
    If Result = False Then
        ErrorMessages
    End If
    
End Sub


Private Sub cmdEcho_Click()
If frmMain.cmdAvailable.Caption = AVAIL_PAUSE Then
    MsgBox "You must pause training before executing the Echo Test"
    Exit Sub
End If

blnExitOnCloseTest = False
ShowAudioTest

End Sub

Private Sub SliderMicVolume_Click()

End Sub

Private Sub Frame8_DragDrop(Source As Control, X As Single, Y As Single)

End Sub
