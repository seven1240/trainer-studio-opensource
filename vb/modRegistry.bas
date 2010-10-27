Attribute VB_Name = "modRegistry"
Option Explicit

    Public Const REG_FOLDER = "Software\Idapted\TrainerClient\"
   Public Const REG_SZ As Long = 1
   Public Const REG_DWORD As Long = 4

   Public Const HKEY_CLASSES_ROOT = &H80000000
   Public Const HKEY_CURRENT_USER = &H80000001
   Public Const HKEY_LOCAL_MACHINE = &H80000002
   Public Const HKEY_USERS = &H80000003

   Public Const ERROR_NONE = 0
   Public Const ERROR_BADDB = 1
   Public Const ERROR_BADKEY = 2
   Public Const ERROR_CANTOPEN = 3
   Public Const ERROR_CANTREAD = 4
   Public Const ERROR_CANTWRITE = 5
   Public Const ERROR_OUTOFMEMORY = 6
   Public Const ERROR_ARENA_TRASHED = 7
   Public Const ERROR_ACCESS_DENIED = 8
   Public Const ERROR_INVALID_PARAMETERS = 87
   Public Const ERROR_NO_MORE_ITEMS = 259

   Public Const KEY_QUERY_VALUE = &H1
   Public Const KEY_SET_VALUE = &H2
   Public Const KEY_ALL_ACCESS = &H3F
   Declare Function RegCloseKey Lib "advapi32.dll" _
   (ByVal hKey As Long) As Long
   Declare Function RegCreateKeyEx Lib "advapi32.dll" Alias _
   "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, _
   ByVal Reserved As Long, ByVal lpClass As String, ByVal dwOptions _
   As Long, ByVal samDesired As Long, ByVal lpSecurityAttributes _
   As Long, phkResult As Long, lpdwDisposition As Long) As Long
   Declare Function RegOpenKeyEx Lib "advapi32.dll" Alias _
   "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, _
   ByVal ulOptions As Long, ByVal samDesired As Long, phkResult As _
   Long) As Long
   Declare Function RegQueryValueExString Lib "advapi32.dll" Alias _
   "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As _
   String, ByVal lpReserved As Long, lpType As Long, ByVal lpData _
   As String, lpcbData As Long) As Long
   Declare Function RegQueryValueExLong Lib "advapi32.dll" Alias _
   "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As _
   String, ByVal lpReserved As Long, lpType As Long, lpData As _
   Long, lpcbData As Long) As Long
   Declare Function RegQueryValueExNULL Lib "advapi32.dll" Alias _
   "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As _
   String, ByVal lpReserved As Long, lpType As Long, ByVal lpData _
   As Long, lpcbData As Long) As Long
   Declare Function RegSetValueExString Lib "advapi32.dll" Alias _
   "RegSetValueExA" (ByVal hKey As Long, ByVal lpValueName As String, _
   ByVal Reserved As Long, ByVal dwType As Long, ByVal lpValue As _
   String, ByVal cbData As Long) As Long
   Declare Function RegSetValueExLong Lib "advapi32.dll" Alias _
   "RegSetValueExA" (ByVal hKey As Long, ByVal lpValueName As String, _
   ByVal Reserved As Long, ByVal dwType As Long, lpValue As Long, _
   ByVal cbData As Long) As Long
   

   
      Public Function SetValueEx(ByVal hKey As Long, sValueName As String, _
   lType As Long, vValue As Variant) As Long
       Dim lValue As Long
       Dim sValue As String
       Select Case lType
           Case REG_SZ
               sValue = vValue & Chr$(0)
               SetValueEx = RegSetValueExString(hKey, sValueName, 0&, _
                                              lType, sValue, Len(sValue))
           Case REG_DWORD
               lValue = vValue
               SetValueEx = RegSetValueExLong(hKey, sValueName, 0&, _
   lType, lValue, 4)
           End Select
   End Function

   Function QueryValueEx(ByVal lhKey As Long, ByVal szValueName As _
   String, vValue As Variant) As Long
       Dim cch As Long
       Dim lrc As Long
       Dim lType As Long
       Dim lValue As Long
       Dim sValue As String

       On Error GoTo QueryValueExError

       ' Determine the size and type of data to be read
       lrc = RegQueryValueExNULL(lhKey, szValueName, 0&, lType, 0&, cch)
       If lrc <> ERROR_NONE Then Error 5

       Select Case lType
           ' For strings
           Case REG_SZ:
               sValue = String(cch, 0)

   lrc = RegQueryValueExString(lhKey, szValueName, 0&, lType, _
   sValue, cch)
               If lrc = ERROR_NONE Then
                   vValue = Left$(sValue, cch - 1)
               Else
                   vValue = Empty
               End If
           ' For DWORDS
           Case REG_DWORD:
   lrc = RegQueryValueExLong(lhKey, szValueName, 0&, lType, _
   lValue, cch)
               If lrc = ERROR_NONE Then vValue = lValue
           Case Else
               'all other data types not supported
               lrc = -1
       End Select

QueryValueExExit:
       QueryValueEx = lrc
       Exit Function

QueryValueExError:
       Resume QueryValueExExit
   End Function
   Public Function SaveKey(ByVal key As String, ByVal value As String)
    SetKeyValue REG_FOLDER, key, value, REG_SZ
    
End Function
   Public Function GetOrCreateRegKey(ByVal key As String, ByVal default As String)
    Dim Ret As String
    Ret = QueryValue(REG_FOLDER, key)
    If Ret = "" Then
        CreateNewKey REG_FOLDER, HKEY_CURRENT_USER
        SetKeyValue REG_FOLDER, key, default, REG_SZ
        GetOrCreateRegKey = default
    Else
        GetOrCreateRegKey = Ret
    End If
    
End Function
Public Function GetKey(ByVal key As String)
  GetKey = QueryValue(REG_FOLDER, key)

End Function
 Private Sub CreateNewKey(sNewKeyName As String, lPredefinedKey As Long)
       Dim hNewKey As Long         'handle to the new key
       Dim lRetVal As Long         'result of the RegCreateKeyEx function

       lRetVal = RegCreateKeyEx(lPredefinedKey, sNewKeyName, 0&, _
                 vbNullString, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, _
                 0&, hNewKey, lRetVal)
       RegCloseKey (hNewKey)
   End Sub
   Private Sub SetKeyValue(sKeyName As String, sValueName As String, _
   vValueSetting As Variant, lValueType As Long)
       Dim lRetVal As Long         'result of the SetValueEx function
       Dim hKey As Long         'handle of open key

       'open the specified key
       lRetVal = RegOpenKeyEx(HKEY_CURRENT_USER, sKeyName, 0, _
                                 KEY_SET_VALUE, hKey)
       lRetVal = SetValueEx(hKey, sValueName, lValueType, vValueSetting)
       RegCloseKey (hKey)
   End Sub
    Private Function QueryValue(sKeyName As String, sValueName As String)
       Dim lRetVal As Long         'result of the API functions
       Dim hKey As Long         'handle of opened key
       Dim vValue As Variant      'setting of queried value

       lRetVal = RegOpenKeyEx(HKEY_CURRENT_USER, sKeyName, 0, _
   KEY_QUERY_VALUE, hKey)
       lRetVal = QueryValueEx(hKey, sValueName, vValue)
       RegCloseKey (hKey)
       QueryValue = vValue
   End Function
   
   Public Sub ResetStudioChineseSettings()

   End Sub
   
   
   
