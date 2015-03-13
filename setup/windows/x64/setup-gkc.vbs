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

'install
Dim strTitle
strTitle = "GKC-1.0.1-win64"

WScript.Echo "Install..."

Dim fso
Set fso = WScript.CreateObject("Scripting.FileSystemObject")
Dim objShell
Set objShell = WScript.CreateObject("Shell.Application")
Dim strSrc, strDest, strAsm
strSrc = strTitle & ".zip"
strDest = "C:\program files\.GKC"
If Not fso.FileExists(strSrc) Then
	WScript.Echo "Error"
	WScript.Quit
End If
If Not fso.FolderExists(strDest) Then
	fso.CreateFolder(strDest)
End If
Set objSource = objShell.NameSpace(fso.GetAbsolutePathName(strSrc))
Set objFolderItem = objSource.Items()
Set objTarget = objShell.NameSpace(strDest)
objTarget.CopyHere objFolderItem, 4 + 512 + 1024

'environment
Set objWsh = WScript.CreateObject("WScript.Shell")
Set objEnv = objWsh.Environment("SYSTEM")
Dim strPath, iLen
strPath = CStr(objEnv("PATH"))
iLen = Len(strPath)
strAsm = strDest & "\" & strTitle & "\public\assemblies"
strDest = Chr(34) & strAsm & Chr(34)
Dim pos
pos = InStr(1, strPath, strDest, 1)
If pos = 0 Then
	If iLen <> 0 Then
		If Right(strPath, 1) <> ";" Then
			strPath = strPath & ";"
		End If
		strPath = strPath & strDest
	Else
		strPath = strDest
	End If
	objEnv("PATH") = strPath
End If

'services
objWsh.Run Chr(34) & strAsm & "\gss-setup.bat" & Chr(34), 0, true
Wscript.Sleep 500

Set objEnv = Nothing
Set objWsh = Nothing
Set objTarget = Nothing
Set objFolderItem = Nothing
Set objSource = Nothing
Set objShell = Nothing
Set fso = Nothing

WScript.Echo "Done" & vbCrLf
