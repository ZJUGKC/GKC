'
' Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
' All rights reserved.
'
' This program is free software; you can redistribute it and/or
' modify it under the terms of the 2-Clause BSD License.
'
' Author contact information:
'   yxxinyuan@zju.edu.cn
'

'uninstall
Dim strTitle
strTitle = "GKC-1.0.1-win64"

WScript.Echo "Uninstall..."

Dim strDest, strEnv, strAsm
strDest = "C:\program files\.GKC"
strAsm = strDest & "\" & strTitle & "\public\assemblies"
strEnv = Chr(34) & strAsm & Chr(34)

'services
Set objWsh = WScript.CreateObject("WScript.Shell")
objWsh.Run Chr(34) & strAsm & "\ghs-uninstall.bat" & Chr(34), 0, true
Wscript.Sleep 500

Dim fso
Set fso = WScript.CreateObject("Scripting.FileSystemObject")
fso.DeleteFolder strDest, True

'environment
Set objEnv = objWsh.Environment("SYSTEM")
Dim strPath, iEnvLen, iLen
strPath = CStr(objEnv("PATH"))
iLen = Len(strPath)
iEnvLen = Len(strEnv)
Dim pos, pos1
pos = InStr(1, strPath, strEnv, 1)
If pos <> 0 Then
	pos1 = pos + iEnvLen
	If pos1 <= iLen Then
		If Mid(strPath, pos1, 1) = ";" Then
			pos1 = pos1 + 1
		End If
	End If
	strPath = Left(strPath, pos - 1) & Right(strPath, iLen - pos1 + 1)
	'delete ;
	iLen = Len(strPath)
	If iLen <> 0 Then
		If Right(strPath, 1) = ";" Then
			strPath = Left(strPath, iLen - 1)
		End If
	End If
	objEnv("PATH") = strPath
End If

Set objEnv = Nothing
Set objWsh = Nothing
Set fso = Nothing

WScript.Echo "Done" & vbCrLf
