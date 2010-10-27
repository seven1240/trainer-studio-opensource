Attribute VB_Name = "modLaunchIE"
Option Explicit
Const HKEY_CLASSES_ROOT = &H80000000
Const HKEY_CURRENT_USER = &H80000001
Const HKEY_LOCAL_MACHINE = &H80000002
Const HKEY_USERS = &H80000003

    ' Reg Key Security Options...
Const KEY_QUERY_VALUE = &H1&
Const KEY_SET_VALUE = &H2&
Const KEY_CREATE_SUB_KEY = &H4&
Const KEY_ENUMERATE_SUB_KEYS = &H8&
Const KEY_NOTIFY = &H10&
Const KEY_CREATE_LINK = &H20&
Const READ_CONTROL = &H20000
Const WRITE_DAC = &H40000
Const WRITE_OWNER = &H80000
Const SYNCHRONIZE = &H100000
Const STANDARD_RIGHTS_REQUIRED = &HF0000
Const STANDARD_RIGHTS_READ = READ_CONTROL
Const STANDARD_RIGHTS_WRITE = READ_CONTROL
Const STANDARD_RIGHTS_EXECUTE = READ_CONTROL
Const KEY_READ = STANDARD_RIGHTS_READ Or KEY_QUERY_VALUE Or KEY_ENUMERATE_SUB_KEYS Or KEY_NOTIFY
Const KEY_WRITE = STANDARD_RIGHTS_WRITE Or KEY_SET_VALUE Or KEY_CREATE_SUB_KEY
Const KEY_EXECUTE = KEY_READ
Const KEY_ALL_ACCESS = KEY_QUERY_VALUE + KEY_SET_VALUE + _
                       KEY_CREATE_SUB_KEY + KEY_ENUMERATE_SUB_KEYS + _
                       KEY_NOTIFY + KEY_CREATE_LINK + READ_CONTROL

Declare Function RegCloseKey& Lib "advapi32.dll" (ByVal hKey&)
Declare Function RegOpenKeyExA& Lib "advapi32.dll" (ByVal hKey&, ByVal lpszSubKey$, dwOptions&, ByVal samDesired&, lpHKey&)
Declare Function RegQueryValueExA& Lib "advapi32.dll" (ByVal hKey&, ByVal lpszValueName$, ByVal lpdwRes&, lpdwType&, ByVal lpDataBuff$, nSize&)
Declare Function RegQueryValueEx& Lib "advapi32.dll" Alias "RegQueryValueExA" (ByVal hKey&, ByVal lpszValueName$, ByVal lpdwRes&, lpdwType&, lpDataBuff&, nSize&)

Const ERROR_SUCCESS = 0&
Const REG_SZ = 1&                          ' Unicode nul terminated string
Const REG_DWORD = 4&                       ' 32-bit number

Function LaunchIE(strURL)

Dim strSubKey As String
Dim strValueName As String
Dim AppPath As String
Dim AppPathLen As Integer

'Registry value locations
strSubKey = "SOFTWARE\Classes\Applications\iexplore.exe\shell\open\command"
strValueName = ""

'Returning the application path back from registry
AppPath = RegGetValueIE$(HKEY_LOCAL_MACHINE, strSubKey, strValueName)


'Determining the length of the path
AppPathLen = Len(AppPath)
'Removing the " %1" from the end of the path
AppPath = Left(AppPath, AppPathLen - 3)
'Adding the site to be navigated to
AppPath = AppPath & " " & strURL

'Launching IE
Shell AppPath, vbNormalFocus

End Function

Function RegGetValueIE$(MainKey&, SubKey$, value$)
   ' MainKey must be one of the Publicly declared HKEY constants.
   Dim sKeyType&       'to return the key type.  This function expects REG_SZ or REG_DWORD
   Dim Ret&            'returned by registry functions, should be 0&
   Dim lpHKey&         'return handle to opened key
   Dim lpcbData&       'length of data in returned string
   Dim ReturnedString$ 'returned string value
   Dim ReturnedLong&   'returned long value
   If MainKey >= &H80000000 And MainKey <= &H80000006 Then
      ' Open key
      Ret = RegOpenKeyExA(MainKey, SubKey, 0&, KEY_READ, lpHKey)
      If Ret <> ERROR_SUCCESS Then
         RegGetValueIE = ""
         Exit Function     'No key open, so leave
      End If
      
      ' Set up buffer for data to be returned in.
      ' Adjust next value for larger buffers.
      lpcbData = 255
      ReturnedString = Space$(lpcbData)

      ' Read key
      Ret& = RegQueryValueExA(lpHKey, value, ByVal 0&, sKeyType, ReturnedString, lpcbData)
      If Ret <> ERROR_SUCCESS Then
         RegGetValueIE = ""   'Value probably doesn't exist
      Else
        If sKeyType = REG_DWORD Then
            Ret = RegQueryValueEx(lpHKey, value, ByVal 0&, sKeyType, ReturnedLong, 4)
            If Ret = ERROR_SUCCESS Then RegGetValueIE = CStr(ReturnedLong)
        Else
            RegGetValueIE = Left$(ReturnedString, lpcbData - 1)
        End If
    End If
      ' Always close opened keys.
      Ret = RegCloseKey(lpHKey)
   End If
End Function

