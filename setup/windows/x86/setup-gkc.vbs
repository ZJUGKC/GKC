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

'function
Sub DecompressZipFile(objFSO, objShell, strZipFile, strDestPath)
	Dim objSource, objFolderItem, objTarget
	Set objSource = objShell.NameSpace(objFSO.GetAbsolutePathName(strZipFile))
	Set objFolderItem = objSource.Items()
	Set objTarget = objShell.NameSpace(strDestPath)
	objTarget.CopyHere objFolderItem, 4 + 512 + 1024
	Do
		WScript.Sleep 500
	Loop Until objTarget.Items().Count >= objFolderItem.Count
	Set objTarget = Nothing
	Set objFolderItem = Nothing
	Set objSource = Nothing
End Sub

'package name
Dim strPackageName
strPackageName = "GKC-1.0.1-win32"
'environment variables
Dim strEnvSystemRoot, strEnvLws, strEnvUws
strEnvSystemRoot = "GKC_SYSTEM_ROOT"
strEnvLws = "GKC_LOCAL_WORKSPACE"
strEnvUws = "GKC_UNIFIED_WORKSPACE"

'command parameters
Dim iCount
iCount = WScript.Arguments.Count
If iCount < 1 Or iCount > 9 Then
	WScript.Echo "Usage: cscript setup-gkc.vbs <-c|-d|-s> [-i Installation-directory] [-l Local-workspace-directory] [-u Unified-workspace-directory] [-si <user:pwd>]"
	WScript.Quit(1)
End If

Dim iSetupMode
Dim strTemp
strTemp = WScript.Arguments(0)
If strTemp = "-c" Then
	iSetupMode = 0
ElseIf strTemp = "-d" Then
	iSetupMode = 1
ElseIf strTemp = "-s" Then
	iSetupMode = 2
Else
	WScript.Echo "Command error: unknown option!"
	WScript.Quit(1)
End If

If iSetupMode <> 2 Then
	If iCount > 7 Then
		WScript.Echo "Command error: user and password should not be specified on client or development server!"
		WScript.Quit(1)
	End If
End If

Dim iPos
Dim strUser, strPwd
strUser = ""
strPwd  = ""

Dim strTargetDir, strLwsDir, strUwsDir
strTargetDir = "C:\program files"
strLwsDir = strTargetDir & "\" & ".GKC" & "\" & "LWS"
strUwsDir = strTargetDir & "\" & ".GKC" & "\" & "UWS"
Dim iIndex, iLen
iIndex = 1
Do While iIndex < iCount
	strTemp = WScript.Arguments(iIndex)
	If strTemp = "-i" Then
		If iIndex <> 1 Then
			WScript.Echo "Command error: the installation directory should be specified at first!"
			WScript.Quit(1)
		End If
		If iIndex + 1 >= iCount Then
			WScript.Echo "Command error: the installation directory should be specified!"
			WScript.Quit(1)
		End If
		strTargetDir = WScript.Arguments(iIndex + 1)
		iLen = Len(strTargetDir)
		If iLen <> 0 Then
			If Right(strTargetDir, 1) = "\" Then
				strTargetDir = Left(strTargetDir, iLen - 1)
			End If
		End If
		strLwsDir = strTargetDir & "\" & ".GKC" & "\" & "LWS"
		strUwsDir = strTargetDir & "\" & ".GKC" & "\" & "UWS"
	ElseIf strTemp = "-l" Then
		If iIndex + 1 >= iCount Then
			WScript.Echo "Command error: the local workspace directory should be specified!"
			WScript.Quit(1)
		End If
		strLwsDir = WScript.Arguments(iIndex + 1)
	ElseIf strTemp = "-u" Then
		If iIndex + 1 >= iCount Then
			WScript.Echo "Command error: the unified workspace directory should be specified!"
			WScript.Quit(1)
		End If
		strUwsDir = WScript.Arguments(iIndex + 1)
	ElseIf strTemp = "-si" Then
		If iSetupMode <> 2 Then
			WScript.Echo "Command error: user and password should not be specified on client or development server!"
			WScript.Quit(1)
		End If
		If iIndex + 1 >= iCount Then
			WScript.Echo "Command error: user and password should be specified!"
			WScript.Quit(1)
		End If
		strUser = WScript.Arguments(iIndex + 1)
		iPos = InStr(1, strUser, ":", 1)
		If iPos = 0 Then
			WScript.Echo "Command error: user or password should not be empty!"
			WScript.Quit(1)
		End If
		strPwd = Right(strUser, Len(strUser) - iPos)
		If Len(strPwd) = 0 Then
			WScript.Echo "Command error: password should not be empty!"
			WScript.Quit(1)
		End If
		strUser = Left(strUser, iPos - 1)
		If Len(strUser) = 0 Then
			WScript.Echo "Command error: user should not be empty!"
			WScript.Quit(1)
		End If
		strUser = ".\" & strUser
	Else
		WScript.Echo "Command error: unknown option for directory!"
		WScript.Quit(1)
	End If
	iIndex = iIndex + 2
Loop

'install
WScript.Echo "Install..."

Dim fso
Set fso = WScript.CreateObject("Scripting.FileSystemObject")
Dim objShell
Set objShell = WScript.CreateObject("Shell.Application")

Dim strSrc
strSrc = strPackageName & ".zip"
If Not fso.FileExists(strSrc) Then
	WScript.Echo "Error: The package file does not exist!"
	WScript.Quit(1)
End If

Dim strDest
strDest = strTargetDir & "\" & ".GKC" & "\" & "SYSTEM"
If fso.FolderExists(strDest) Then
	WScript.Echo "Error: The software is not uninstalled!"
	WScript.Quit(1)
End If
strDest = strTargetDir & "\" & ".GKC"

Set objWsh = WScript.CreateObject("WScript.Shell")
Set objEnv = objWsh.Environment("SYSTEM")

strTemp = CStr(objEnv(strEnvSystemRoot))
If Len(strTemp) <> 0 Then
	WScript.Echo "Error: The software is not uninstalled!"
	WScript.Quit(1)
End If
strTemp = CStr(objEnv(strEnvLws))
If Len(strTemp) <> 0 Then
	WScript.Echo "Error: The software is not uninstalled!"
	WScript.Quit(1)
End If
strTemp = CStr(objEnv(strEnvUws))
If Len(strTemp) <> 0 Then
	WScript.Echo "Error: The software is not uninstalled!"
	WScript.Quit(1)
End If

If Not fso.FolderExists(strDest) Then
	fso.CreateFolder(strDest)
End If
DecompressZipFile fso, objShell, strSrc, strDest
fso.GetFolder(strDest & "\" & strPackageName).Name = "SYSTEM"
strDest = strDest & "\" & "SYSTEM"

If iSetupMode = 0 Then
	strTemp = strDest & "\" & "core" & "\" & "services"
	fso.DeleteFolder strTemp, true
	strTemp = strDest & "\" & "development"
	fso.DeleteFolder strTemp, true
ElseIf iSetupMode = 1 Then
	strTemp = strDest & "\" & "core" & "\" & "services"
	fso.DeleteFolder strTemp, true
	strTemp = strDest & "\" & "client"
	fso.DeleteFolder strTemp, true
	strTemp = strDest & "\" & "software"
	If fso.FolderExists(strTemp) Then
		fso.DeleteFolder strTemp, true
	End If
ElseIf iSetupMode = 2 Then
	strTemp = strDest & "\" & "core" & "\" & "tools"
	fso.DeleteFolder strTemp, true
	strTemp = strDest & "\" & "development"
	fso.DeleteFolder strTemp, true
	strTemp = strDest & "\" & "client"
	fso.DeleteFolder strTemp, true
End If

If Not fso.FolderExists(strLwsDir) Then
	fso.CreateFolder(strLwsDir)
End If
If Not fso.FolderExists(strUwsDir) Then
	fso.CreateFolder(strUwsDir)
End If

'environment
objEnv(strEnvSystemRoot) = strDest
objEnv(strEnvLws) = strLwsDir
objEnv(strEnvUws) = strUwsDir

strTemp = strDest & "\" & "core" & "\" & "assemblies"
If iSetupMode <> 2 Then
	strTemp = strTemp & ";" & strDest & "\" & "core" & "\" & "tools"
End If
If iSetupMode = 0 Then
	strTemp = strTemp & ";" & strDest & "\" & "client"
End If
If iSetupMode = 1 Then
	strTemp = strTemp & ";" & strDest & "\" & "development" & "\" & "tools"
End If

Dim strPath
strPath = CStr(objEnv("PATH"))
iLen = Len(strPath)
iPos = InStr(1, strPath, strDest, 1)
If iPos = 0 Then
	If iLen <> 0 Then
		If Right(strPath, 1) <> ";" Then
			strPath = strPath & ";"
		End If
		strPath = strPath & strTemp
	Else
		strPath = strTemp
	End If
	objEnv("PATH") = strPath
End If

'services
If iSetupMode = 2 Then
	strTemp = Chr(34) & strDest & "\" & "core" & "\" & "services" & "\" & "ghs-setup.bat" & Chr(34) _
			& " " & Chr(34) & strDest & "\" & "core" & "\" & "services" & "\" & "GkcHostSvc.exe" & Chr(34)
	If Len(strUser) <> 0 Then
		strTemp = strTemp & " " & Chr(34) & strUser & Chr(34) & " " & Chr(34) & strPwd & Chr(34)
	End If
	objWsh.Run strTemp, 0, true
	Wscript.Sleep 500
End If

Set objEnv = Nothing
Set objWsh = Nothing
Set objShell = Nothing
Set fso = Nothing

'done
WScript.Echo "Done" & vbCrLf
