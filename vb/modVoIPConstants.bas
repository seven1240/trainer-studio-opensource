Attribute VB_Name = "modVoIP"

Public m_MinRTPPort As Long, m_MaxRTPPort As Long, m_RTPPort As Long

Public Const VOIP_DEFAULT_CODEC = 3

Public Const ISAC = 103
Public Const iLBC = 97
Public Const iLBC102 = 102
Public Const G729 = 18

Public Const WAIT_TO_RESPONSE = 2
Public Const ID183CODE = 180
Public Const ID180CODE = 183
Public Const ID200CODE = 200
Public Const SEND401 = 401
Public Const BALANCE_MSG = 402
Public Const NOT_FOUND_MSG = 404
Public Const SEND407 = 407
Public Const CALL_DOES_NOT_EXIST = 481
Public Const ADDRESS_ICOMPLETE_MSG = 484
Public Const BUSY_MSG = 486
Public Const ID200BYECODE = 1000
Public Const ID200CANCELCODE = 2000
Public Const IDLECODE = 2001
Public Const INBOUNDCANCELCODE = 2002
Public Const NOTIFYNEWMAIL = 2003
Public Const NOTIFYNONEWMAIL = 2004
Public Const B_SIDE_DISCONNECT = 2005
Public Const INBOUND_CALL = 3000
Public Const GIPSSIPVOICE_ADDTOCONFERENCE = 5000
Public Const NOTIFICAITON_GIPS_WAVE_OUT = 5001
Public Const NOTIFICAITON_GIPS_WAVE_IN = 5002
Public Const IDS_SOCK_WSANOTINITIALISED_ERR = 5003
Public Const IDS_SOCK_WSAENETDOWN_ERR = 5004
Public Const IDS_SOCK_7ERR = 5005
Public Const IDS_SOCK_WSAEADDRNOTAVAIL_ERR = 5006
Public Const IDS_SOCK_WSAEFAULT_ERR = 5007
Public Const IDS_SOCK_WSAEINPROGRESS_ERR = 5008
Public Const IDS_SOCK_WSAEINVAL_ERR = 5009
Public Const IDS_SOCK_WSAENOBUFS_ERR = 5010


Public Const INIT_SUCCEED = 5010
Public Const IDS_STUN_ERRR = 5012
Public Const IDS_SYMMETRIC_NAT = 5013
Public Const IDS_FIREWALL = 5014

Public Const INIT_FAILED = 2222
Private Declare Function MessageBox Lib "user32" _
   Alias "MessageBoxA" _
  (ByVal hWnd As Long, _
   ByVal lpText As String, _
   ByVal lpCaption As String, _
   ByVal wType As Long) As Long
   
Public VoipPassword As String
Public VoipLogin As String
Public VoipProxy As String
Public VoipPort As String
Public SkipEchoTest As String
Public FinishedWithPromptScreen As Boolean
   
   
Public Sub SetProfile()

'    frmMain.oVoIP.SetProfile VoipLogin & "~" & VoipPassword, frmAdvanceLogin.txtSipProxy, frmAdvanceLogin.txtListeningPort, 7070, 16000, 65000, iLBC102, frmAdvanceLogin.txtStunServer, frmAdvanceLogin.txtStunPort, frmAdvanceLogin.chkStun, AudioOutDev, AudioInDev, frmAdvanceLogin.txtVPNDNS, frmAdvanceLogin.txtVPNServerPort, frmAdvanceLogin.txtVPNPort, frmAdvanceLogin.chkUseVPN
    frmMain.oVoIP.SetProfile VoipLogin & "~" & VoipPassword, VoipProxy, CLng(VoipPort), CLng(frmAdvanceLogin.txtLocalVoIPPort), CLng(frmAdvanceLogin.txtRTPMin), CLng(frmAdvanceLogin.txtRTPMax), iLBC102, "stun.idapted.com", 1000, False, AudioOutDev, AudioInDev, "nothing", 80, 80, False



End Sub
Public Sub Disconnect()
'    On Error Resume Next
    log_it "Private DISCONNECT!"

   ' SetPresence "review"
    If frmPromptScreen.cmdDisconnect.Visible = True Then
        log_it "Disconnect executing..."
        frmMain.oVoIP.Hangup
        frmPromptScreen.cmdDisconnect.Visible = False
        frmPromptScreen.cmdReconnect.Visible = False
        frmPromptScreen.tmrReconnect.Enabled = False
        frmPromptScreen.lblReconnecting.Visible = False
        'frmPromptScreen.lblTime.Visible = False
        DoEvents
        Dim re As Long
        If DateDiff("s", frmMain.tmrInteraction.Tag, Time) < 450 Then
            re = MessageBox(frmPromptScreen.hWnd, "This call was ended prematurely.  Would you like to skip the review screen?", "Premature Ending", vbYesNo Or vbDefaultButton2)

            If re = vbYes Then
                FinishedWithPromptScreen = True
                Unload frmPromptScreen

'                 frmPromptScreen.Flash.CallFunction "<invoke name=""popUp"" returntype=""xml""></invoke>"
                Exit Sub
            End If
        End If
        log_it "Disconnect - Stop Flash"
        frmPromptScreen.Flash.Stop
        frmPromptScreen.Flash.LoadMovie 0, "empty.swf"
        log_it "Disconnect - set flashvars"
       ' frmPromptScreen.Flash.Visible = False
        frmPromptScreen.FlashReview.Visible = True
        frmPromptScreen.FlashReview.FlashVars = "base_url=http://" & RemoveHttp(frmAdvanceLogin.txtAppRoot) & "&interaction_id=" & frmPromptScreen.Tag & "&product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer"
        log_it "Disconnect load movie"
        frmPromptScreen.FlashReview.LoadMovie 0, FlashReviewUrl
        frmPromptScreen.tmrReview.Tag = Time
        frmPromptScreen.tmrReview.Enabled = True
        SendAction "Review"
        
    End If
End Sub
Public Sub Logoff()
    frmMain.oVoIP.Logoff
End Sub
Public Sub Login()

log_it "Logging in"




log_it "A" & VoipPassword & "A"
Dim ListenPort As Integer
  
    ListenPort = 7070
    
    If Not (Len(Trim(VoipLogin)) = 0 Or Len(Trim(VoipPassword)) = 0) Then
    'profile data ok
        Screen.MousePointer = 11
        
        SetLoginStatus "Loading Components..."
        
        frmMain.oVoIP.Logoff
        frmMain.oVoIP.UnInitVELibrary
        
        frmMain.oVoIP.InitVELibrary
        
        SetProfile
        m_MinRTPPort = 16000
        m_MaxRTPPort = 65000
        m_RTPPort = m_MinRTPPort
                        
        frmMain.oVoIP.SetAGCStatus 1
        frmMain.oVoIP.SetNRStatus 1
        frmMain.oVoIP.SetECStatus 1
    
        SetLoginStatus "Registering to SIP Proxy."

        frmMain.oVoIP.SetDIDsList VoipLogin
     
        
        Screen.MousePointer = 0
                
    Else
    
          
        SetLoginError "Invalid Login Information"
        
        Exit Sub
End If
    
  
End Sub
Private Sub GetRTPPort()

    m_RTPPort = m_RTPPort + 2
    
    If m_RTPPort > m_MaxRTPPort Then
    
        m_RTPPort = m_MinRTPPort
        
    End If
    
End Sub
Public Sub DialNumber(num As String)

  CallNumber num
  frmMain.fraDial.Visible = True
  frmMain.fraMenu.Visible = False
  frmMain.lblDialStatus.Caption = "Dialing..."
  
End Sub

Public Sub CallNumber(num As String)
     frmMain.oVoIP.Hangup
     GetRTPPort
     frmMain.oVoIP.MakeCall num, m_RTPPort, iLBC102
End Sub


Public Sub SocketAuthenticate()
    Dim sResults As String
    Dim LineEnd As String
    
    LineEnd = "',"
   SetLoginStatus "Checking version and login information"
   'Get system information
    sResults = ""
    sResults = sResults & "'memory': '" & frmMain.SysInfo.MemorySize / 1024 & LineEnd
sResults = sResults & "'screen_res': '" & frmMain.SysInfo.ScreenResolution & LineEnd
sResults = sResults & "'flash_player_version': '" & frmMain.FlashLoad.GetVariable("$version") & LineEnd
''Can be Vista/WinXP/Win2K/Win95/Win98/WinMe/WinNT/Unknown
sResults = sResults & "'os': '" & frmMain.SysInfo.GetOS & "'"


   
   frmMain.Sock.Tag = "Authenticate"
   frmMain.Sock.SendData Quotize("{'action': 'Authenticate','username': '" + frmLogin.txtUsername.Text + "','password': '" + frmLogin.txtPassword.Text + "','system_info': [{" & sResults + "}]}")
   
End Sub

Public Function LoginToVoip(params As json) As Boolean
    'On Error GoTo err
  LoginToVoip = True
  
  If params.Fetch("status") = "AuthenticateError" Then
    SetLoginError "Unable to login: " + params.Fetch("reason")
    
  ElseIf params.Fetch("status") = "Authenticated" Then
  
    Dim upgrade As String
    upgrade = CheckUpgrade(params.Fetch("version"))
        
        
    If upgrade = "Error" Then
        MsgBox "Faltal error when getting version, contact tech support please"
        LoginToVoip = False
        Exit Function
    ElseIf upgrade = "Forced" Then
        MsgBox "A new version of the Idapted Trainer Client has been released.Please close this and login to the trainer portal to download the new version (" & params.Fetch("version") & ")."
        'UnloadAll
        LoginToVoip = False
        Exit Function
    ElseIf upgrade = "Recommended" Then
        If MsgBox("A new version of the Idapted Trainer Client has been released, we suggest you use the new version to get bug fixes and new features! however, you can still work with this version. Do you want to continue work with this version ?", vbYesNo) = vbNo Then
            LoginToVoip = False
            Exit Function
        End If
    End If
    
    VoipLogin = params.Fetch("voip_username")
    UserType = "eqenglish"
    VoipPassword = params.Fetch("voip_password")
    SkipEchoTest = params.Fetch("skip_echo_test")
    VoipProxy = params.Fetch("servers")(0).Fetch("sip_proxy")
    VoipPort = params.Fetch("servers")(0).Fetch("sip_port")
    Login
    
    Exit Function
  End If
  
err:
    SetLoginError "Unable to login: " + params.Fetch("reason")
  End Function


Public Function CheckUpgrade(version As String) As String
    
    parts = Split(version, ".")
    If UBound(parts) < 2 Then
        CheckUpgrade = "Error"
        Exit Function
    End If
    
    
    Dim major As Integer
    Dim minor As Integer
    Dim rev As Integer
    Dim upgrade As String
    
    major = CInt(parts(0))
    minor = CInt(parts(1))
    rev = CInt(parts(2))
    
    If major > App.major Then
        upgrade = "Forced"
    ElseIf minor > App.minor Then
        upgrade = "Forced"
    ElseIf rev > App.Revision Then
        upgrade = "Recommended"
    Else
        upgrade = "NO"
    End If
    
    CheckUpgrade = upgrade

End Function

