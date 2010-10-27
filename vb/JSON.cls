VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "JSON"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'This is singularly the stupidest code imaginable

Private Str As String
Private Keys() As String
Private Values() As String


Public Sub Decode(inp)

Dim inkey As Boolean, invalue As Boolean

Dim numElements As Long

Str = inp
inkey = False
invalue = False
n = 1
Str = Trim(Str)

If Right(Str, 1) = Chr(13) Or Right(Str, 1) = Chr(10) Then
     ' /* Should be Left, Not Right */
     Str = Left(Str, Len(Str) - 1)
End If
Str = Right(Str, Len(Str) - 1)
Str = Left(Str, Len(Str) - 1)

numElements = numElements + 1


Do While n <= Len(Str)
    'lets find thes start of the key
    keystart = InStr(n, Str, Chr(34)) + 1
    keyend = InStr(keystart, Str, Chr(34))
    
    nextquote = InStr(keyend + 1, Str, Chr(34))
    If nextquote = 0 Then nextquote = Len(Str) + 1
    nextcomma = InStr(keyend + 1, Str, ",")
    If nextcomma = 0 Then nextcomma = Len(Str) + 1
    nextbracket = InStr(keyend + 1, Str, "[")
    If nextbracket = 0 Then nextbracket = Len(Str) + 1

    If (nextquote < nextcomma And nextquote < nextbracket) Then
        'We have a quoted value
        valuestart = nextquote + 1
        valueend = InStr(valuestart, Str, Chr(34) + ",")
    
        If valueend = 0 Then valueend = Len(Str)
    ElseIf nextbracket < nextcomma Then
        valuestart = nextbracket + 1
        valueend = InStr(valuestart, Str, "}]") + 1
    
    Else
        valuestart = keyend + 2
        valueend = InStr(valuestart + 1, Str, ",")
            
        If valueend = 0 Then valueend = Len(Str) + 1
           
        
    End If

    
    n = valueend + 1
    key = Mid(Str, keystart, keyend - keystart)
    value = Mid(Str, valuestart, valueend - valuestart)
    numElements = numElements + 1
    ReDim Preserve Keys(numElements)
    ReDim Preserve Values(numElements)
  
    Keys(numElements - 1) = key
    Values(numElements - 1) = value
    
    

Loop

End Sub


Public Function Fetch(key)
    For i = 0 To UBound(Keys)
        If Keys(i) = key Then
        
            If InStr(1, Values(i), "{") = 1 And InStr(1, Values(i), "}") = Len(Values(i)) Then
                Dim jsons() As json, parts() As String
                parts = Split(Values(i), "}, {")
                ReDim jsons(UBound(parts))
                
        
                For n = 0 To UBound(parts)
                    Set jsons(n) = New json
                    
                    part = parts(n)
                    
                    If Not InStr(1, part, "{") = 1 Then part = "{" + part
                    If Not InStr(1, part, "}") = Len(part) Then part = part + "}"
               
                    jsons(n).Decode part
                Next n
                Fetch = jsons
                Exit For
            Else
                Fetch = Values(i)
                 Exit For
            End If
        End If
    Next i
        
End Function



