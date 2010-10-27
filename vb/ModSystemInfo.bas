Attribute VB_Name = "ModSystemInfo"
Option Explicit

Public Declare Function FlashWindowEx Lib "user32" _
   (FWInfo As FLASHWINFO) As Boolean

Public Declare Sub Sleep Lib "kernel32" _
   (ByVal dwMilliseconds As Long)

Public Type FLASHWINFO
  cbSize As Long     ' size of structure
  hWnd As Long       ' hWnd of window to use
  dwFlags As Long    ' Flags, see below
  uCount As Long     ' Number of times to flash window
  dwTimeout As Long  ' Flash rate of window in milliseconds. 0 is default.
End Type

Private Const FLASHW_STOP = 0
Private Const FLASHW_CAPTION = 1
Private Const FLASHW_TRAY = 2
Private Const FLASHW_ALL = FLASHW_CAPTION Or FLASHW_TRAY
Private Const FLASHW_TIMER = 4
Private Const FLASHW_TIMERNOFG = 12

Public Sub flash_taskbar_icon(hWnd)
   Dim RetVal As Integer
   Dim FWInfo As FLASHWINFO
   
   ' Fill the structure:
   With FWInfo
      .cbSize = 20
      .hWnd = hWnd
      .dwFlags = FLASHW_ALL Or FLASHW_TIMER
      .uCount = 0
      .dwTimeout = 5000
   End With
   
   RetVal = FlashWindowEx(FWInfo)
End Sub

Public Sub unflash_taskbar_icon(hWnd)
   Dim RetVal As Integer
   Dim FWInfo As FLASHWINFO
   
   ' Fill the structure:
   With FWInfo
      .cbSize = 20
      .hWnd = hWnd
      .dwFlags = FLASHW_STOP
      .uCount = 0
      .dwTimeout = 0
   End With
   
   RetVal = FlashWindowEx(FWInfo)
End Sub



