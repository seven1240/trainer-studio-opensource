Attribute VB_Name = "modInet"
Public Function HumanizeInetCode(ByVal status As Integer, component As String, responseCode As Long)
Select Case status
     Case icResolvingHost
        HumanizeInetCode = component & ": Resolving Host"
   Case icHostResolved
        HumanizeInetCode = component & ": Host Resolved"
   Case icConnecting
        HumanizeInetCode = component & ": Connecting"
    Case icConnected
        HumanizeInetCode = component & ": Connected"
    Case icRequesting
        HumanizeInetCode = component & ": Requesting"
    Case icRequestSent
        HumanizeInetCode = component & ": Request Sent"
    Case icReceivingResponse
        HumanizeInetCode = component & ": Receiving Response"
    Case icDisconnecting
        HumanizeInetCode = component & ": Disconnecting"
    Case icDisconnected
        HumanizeInetCode = component & ": Disconnected"
    Case icResponseReceived
        HumanizeInetCode = component & ": Response Received"
    Case icError
        Screen.MousePointer = 0
        log_it TranslateInetErrorCode(responseCode)
        SetLoginError "Error connecting to server for " & component & ": " & TranslateInetErrorCode(responseCode)
    Case Else
        HumanizeInetCode = component & ": Unknown Inet Code, " & CStr(status)
End Select
End Function

Public Function TranslateInetErrorCode(ByVal lErrorCode As Long) As String
    Select Case lErrorCode
        Case 0
        Case 12001: TranslateInetErrorCode = "No more handles could be generated at this time"
        Case 12002: TranslateInetErrorCode = "The request has timed out."
        Case 12003: TranslateInetErrorCode = "An extended error was returned from the server."
        Case 12004: TranslateInetErrorCode = "An internal error has occurred."
        Case 12005: TranslateInetErrorCode = "The URL is invalid."
        Case 12006: TranslateInetErrorCode = "The URL scheme could not be recognized, or is not supported."
        Case 12007: TranslateInetErrorCode = "The server name could not be resolved."
        Case 12008: TranslateInetErrorCode = "The requested protocol could not be located."
        Case 12009: TranslateInetErrorCode = "A request to InternetQueryOption or InternetSetOption specified an invalid option value."
        Case 12010: TranslateInetErrorCode = "The length of an option supplied to InternetQueryOption or InternetSetOption is incorrect for the type of option specified."
        Case 12011: TranslateInetErrorCode = "The request option can not be set, only queried. "
        Case 12012: TranslateInetErrorCode = "The Win32 Internet support is being shutdown or unloaded."
        Case 12013: TranslateInetErrorCode = "The request to connect and login to an FTP server could not be completed because the supplied user name is incorrect."
        Case 12014: TranslateInetErrorCode = "The request to connect and login to an FTP server could not be completed because the supplied password is incorrect. "
        Case 12015: TranslateInetErrorCode = "The request to connect to and login to an FTP server failed."
        Case 12016: TranslateInetErrorCode = "The requested operation is invalid. "
        Case 12017: TranslateInetErrorCode = "The operation was canceled, usually because the handle on which the request was operating was closed before the operation completed."
        Case 12018: TranslateInetErrorCode = "The type of handle supplied is incorrect for this operation."
        Case 12019: TranslateInetErrorCode = "The requested operation can not be carried out because the handle supplied is not in the correct state."
        Case 12020: TranslateInetErrorCode = "The request can not be made via a proxy."
        Case 12021: TranslateInetErrorCode = "A required registry value could not be located. "
        Case 12022: TranslateInetErrorCode = "A required registry value was located but is an incorrect type or has an invalid value."
        Case 12023: TranslateInetErrorCode = "Direct network access cannot be made at this time. "
        Case 12024: TranslateInetErrorCode = "An asynchronous request could not be made because a zero context value was supplied."
        Case 12025: TranslateInetErrorCode = "An asynchronous request could not be made because a callback function has not been set."
        Case 12026: TranslateInetErrorCode = "The required operation could not be completed because one or more requests are pending."
        Case 12027: TranslateInetErrorCode = "The format of the request is invalid."
        Case 12028: TranslateInetErrorCode = "The requested item could not be located."
        Case 12029: TranslateInetErrorCode = "The attempt to connect to the server failed."
        Case 12030: TranslateInetErrorCode = "The connection with the server has been terminated."
        Case 12031: TranslateInetErrorCode = "The connection with the server has been reset."
        Case 12036: TranslateInetErrorCode = "The request failed because the handle already exists."
        Case Else: TranslateInetErrorCode = "Error details not available."
    End Select
End Function
