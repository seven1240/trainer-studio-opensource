Attribute VB_Name = "modMain"
Public Username As String
Public Password As String
Public CallerID As String
Public BaseURL As String


Public swParam As Long
Public slParam As Long
   
    '\\ --[VB_WindowProc]---------------------------------
    '\\ 'typedef LRESULT (CALLBACK* WNDPROC)(HWND,
    '\\                     UINT, WPARAM, LPARAM);
    '\\ Parameters:
    '\\   hwnd - window handle receiving message
    '\\   wMsg - The window message (WM_..etc.)
    '\\   wParam - First message parameter
    '\\   lParam - Second message parameter
Public Function VB_WindowProc(ByVal hwnd As Long, ByVal wMsg As Long, ByVal wparam As Long, ByVal lparam As Long) As Long

    If wMsg = WM_IDP_DISCONNECT Then
        Disconnect
    ElseIf wMsg = WM_IDP_TIMER Then
       SetLblTime wparam
    Else
        '\\Pass the message to the previous
        '\\window procedure to handle it
        VB_WindowProc = CallWindowProc(hOldProc, hwnd, _
                                    wMsg, wparam, lparam)
    End If
     
End Function


   '\\ Use (after creating the window...)
Public Sub Disconnect()
    frmBrowser.cmdHangup.Visible = False
    frmBrowser.lblTime.Visible = False
    frmBrowser.lblReview.Visible = True
    frmBrowser.Browser.Navigate2 BaseURL + "/get_review?caller_id=" + CallerID
    SendMsg CLIENT, WM_IDP_DISCONNECT
End Sub
Public Sub SetLblTime(time As Long)
  Dim m As Long
  Dim s As Long
  Dim neg As Boolean
  Dim str As String
  str = "Call Time: "
  m = time \ 60
  s = time Mod 60
  If s < 0 Or m < 0 Then
    s = Abs(s)
    m = Abs(m)
    str = str & "- "
  End If
  
  If time > 30 Then
    SetTimerLabelBackcolor (vbGreen)
  ElseIf time > 0 Then
    SetTimerLabelBackcolor (vbYellow)
  Else
    If frmBrowser.lblTime.BackColor = vbRed Then
      SetTimerLabelBackcolor (vbWhite)
    Else
       SetTimerLabelBackcolor vbRed
    End If
End If
  str = str & Format(m, "00") + ":" + Format(s, "00")
  frmBrowser.lblTime.Caption = str
End Sub
Private Sub SetTimerLabelBackcolor(color As Long)
frmBrowser.lblTime.BackColor = color
End Sub

    
