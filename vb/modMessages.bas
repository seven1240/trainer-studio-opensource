Attribute VB_Name = "modMessages"
Option Explicit
 Private Declare Function PostMessage Lib "user32" Alias "PostMessageA" ( _
    ByVal hwnd As Long, _
    ByVal wMsg As Long, _
    ByVal wparam As Long, _
    ByVal lparam As Long) As Long
Public Declare Function SendMessageLong Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wparam As Long, ByVal lparam As Long) As Long
Public Const GWL_WNDPROC = (-4)
Public Declare Function SetWindowLongApi Lib "user32" _
     Alias "SetWindowLongA" _
    (ByVal hwnd As Long, ByVal nIndex As Long, _
     ByVal dwNewLong As Long) As Long

Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hwnd As Long, ByVal msg As Long, ByVal wparam As Long, ByVal lparam As Long) As Long
'\\ Declaration to register custom messages
Public Declare Function RegisterWindowMessage Lib "user32" Alias _
  "RegisterWindowMessageA" (ByVal lpString As String) As Long
   Public Const MSG_DISCONNECT = "IDP_DISCONNECT"
    Public Const MSG_NEW_TIME = "IDP_TIMER"
    Public Const MSG_PLAYSOUND = "IDP_PLAYSOUND"
    Public Const MSG_RECORD = "IDP_RECORD"
Public Declare Function FindWindow Lib "user32" Alias "FindWindowA" (ByVal lpClassName As String, ByVal lpWindowName As String) As Long

Public Const Browser = "Idapted Interaction Screen"
Public Const CLIENT = "Idapted Trainer Client"
Public hOldProc As Long
 Public Function WM_IDP_TIMER() As Long
      Static msg As Long
      
      If msg = 0 Then
            msg = RegisterWindowMessage(MSG_NEW_TIME)
      End If
     
     WM_IDP_TIMER = msg
     
     End Function

      Public Function WM_IDP_DISCONNECT() As Long
      Static msg As Long
      
      If msg = 0 Then
            msg = RegisterWindowMessage(MSG_DISCONNECT)
      End If
     
     WM_IDP_DISCONNECT = msg
     
     End Function

      Public Function WM_IDP_PLAYSOUND() As Long
      Static msg As Long
      
      If msg = 0 Then
            msg = RegisterWindowMessage(MSG_PLAYSOUND)
      End If
     
     WM_IDP_PLAYSOUND = msg
     
     End Function
 Public Function WM_IDP_RECORD() As Long
    Static msg As Long
      
    If msg = 0 Then
         msg = RegisterWindowMessage(MSG_RECORD)
    End If
     
    WM_IDP_RECORD = msg
     
End Function

Public Sub SetWindowProc(hwnd)
   hOldProc = SetWindowLongApi(hwnd, GWL_WNDPROC, AddressOf VB_WindowProc)

End Sub
Public Function SendMsg(location As String, msg, Optional lparam As Long = 0, Optional wparam As Long = 0)
  Dim hwndTarget As Long
  
  hwndTarget = FindWindow(vbNullString, location)
  
  If hwndTarget <> 0 Then
      Call SendMessageLong(hwndTarget, msg, lparam, wparam)
      SendMsg = True
  Else
    SendMsg = False
  End If
End Function
Public Function SendBrowserMsg(msg, Optional lparam As Long = 0, Optional wparam As Long = 0)
    SendBrowserMsg = SendMsg(Browser, msg, lparam, wparam)
End Function

Public Function GetBrowser()
 If FindWindow(vbNullString, Browser) <> 0 Then
    GetBrowser = True
 Else
    GetBrowser = False
 End If
End Function
