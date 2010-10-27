Attribute VB_Name = "modEchoTest"
Option Explicit
Public blnDetectLevel As Boolean
Public DetectedLevels() As Long
Public blnExitOnCloseTest As Boolean


Public EchoTest As Long

Private tmpSpkVolume As Long
Public Sub ShowAudioTest()
    With frmAudioTest
        .frmBegin.Visible = True
        .frmContinue.Visible = False
        .frmMicTest.Visible = False
        .frmSpkTest.Visible = False
        .frmFinished.Visible = False
        .frmMicError.Visible = False
        .frmSpeakerError.Visible = False
    'If trainer has logged in in last 8hrs then allow skipping of echo test

    If UserIsAdmin Or AlreadyCompletedEchoTest Or SkipEchoTest = "true" Then
      .cmdSkipTest.Visible = True
    Else
        .cmdSkipTest.Visible = False
    End If
    End With
    frmAudioTest.Show vbModal
End Sub
Public Sub BeginEchoTest()
    blnDetectLevel = True
    ReDim DetectedLevels(1)
    EchoTest = 1
    

    
    DialNumber ("10")
    With frmAudioTest
  
     
        .frmMicTest.Visible = True
        .lblMicTest.Caption = "Connecting ..."
        .frmMicDisplay.Visible = False
        .frmBegin.Visible = False
        .frmMicError.Visible = False
    End With
End Sub
Public Sub BeginSpeakerTest()
    EchoTest = 2
    DialNumber ("20")
    With frmAudioTest
        .frmContinue.Visible = False
        .frmSpeakerError.Visible = False
        .frmSpkDisplay.Visible = False
        .frmSpkTest.Visible = True
        .lblSpkTest.Caption = "Connecting..."
    End With
End Sub
Public Sub EchoTestConnect()
    Select Case EchoTest
        Case 1

             'Fix for wierd beep effect
            tmpSpkVolume = frmMain.oVoIP.GetSKVolume()
           ' frmMain.oVoIP.SetMixerVolume 8, 0
    
            frmAudioTest.frmMicDisplay.Visible = True
            frmAudioTest.lblMicTest.Caption = "Say 'Testing 1, 2, 3' Outloud'"
        Case 2
            frmAudioTest.frmSpkDisplay.Visible = True
            frmAudioTest.lblSpkTest.Caption = "Listen to yourself repeated back"
    End Select
End Sub
Public Sub EndMicTest()
        Dim n As Long
    Dim exceeded As Long
    
    If tmpSpkVolume = 0 Then
        tmpSpkVolume = 50
    End If
'    frmMain.oVoIP.SetMixerVolume 8, tmpSpkVolume
    
    exceeded = 0
    blnDetectLevel = False
    For n = 0 To UBound(DetectedLevels)
        If DetectedLevels(n) > 3 Then
           exceeded = exceeded + 1
        End If
    Next n
    
    If exceeded > 3 Then
      frmAudioTest.frmContinue.Visible = True
    Else
      frmAudioTest.frmMicError.Visible = True
    End If
    
    frmAudioTest.frmMicTest.Visible = False
    EchoTest = 0
End Sub
Public Sub EndTest()
 If EchoTest = 1 Then
    EndMicTest
Else
    EndSpeakerTest
End If
End Sub
Public Sub EndSpeakerTest()
    frmAudioTest.frmFinished.Visible = True
    frmAudioTest.frmSpkTest.Visible = False
    EchoTest = 0
    AlreadyCompletedEchoTest = True
End Sub
