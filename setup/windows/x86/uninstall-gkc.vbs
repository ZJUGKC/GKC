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

'environment variables
Dim strEnvSystemRoot, strEnvLws, strEnvUws
strEnvSystemRoot = "GKC_SYSTEM_ROOT"
strEnvLws = "GKC_LOCAL_WORKSPACE"
strEnvUws = "GKC_UNIFIED_WORKSPACE"

'command parameters
Dim iCount
iCount = WScript.Arguments.Count
If iCount > 1 Then
	WScript.Echo "Usage: cscript uninstall-gkc.vbs [-a]"
	WScript.Quit(1)
End If

Dim iUninstallMode
Dim strTemp
iUninstallMode = 0
If iCount = 1 Then
	strTemp = WScript.Arguments(0)
	If strTemp = "-a" Then
		iUninstallMode = 1
	Else
		WScript.Echo "Command error: unknown option!"
		WScript.Quit(1)
	End If
End If

'uninstall
WScript.Echo "Uninstall..."

Dim fso
Set fso = WScript.CreateObject("Scripting.FileSystemObject")

'environment
Set objWsh = WScript.CreateObject("WScript.Shell")
Set objEnv = objWsh.Environment("SYSTEM")

Dim strInstallDir, strLwsDir, strUwsDir
strInstallDir = CStr(objEnv(strEnvSystemRoot))
If Len(strInstallDir) = 0 Then
	WScript.Echo "Error: The software is not installed!"
	WScript.Quit(1)
End If
strLwsDir = CStr(objEnv(strEnvLws))
If Len(strLwsDir) = 0 Then
	WScript.Echo "Error: The software is not installed!"
	WScript.Quit(1)
End If
strUwsDir = CStr(objEnv(strEnvUws))
If Len(strUwsDir) = 0 Then
	WScript.Echo "Error: The software is not installed!"
	WScript.Quit(1)
End If

If Not fso.FolderExists(strInstallDir) Then
	WScript.Echo "Error: The software is not installed!"
	WScript.Quit(1)
End If
If Not fso.FolderExists(strLwsDir) Then
	WScript.Echo "Error: The software is not installed!"
	WScript.Quit(1)
End If
If Not fso.FolderExists(strUwsDir) Then
	WScript.Echo "Error: The software is not installed!"
	WScript.Quit(1)
End If

'services
strTemp = strInstallDir & "\" & "core" & "\" & "services"
If fso.FolderExists(strTemp) Then
	objWsh.Run Chr(34) & strTemp & "\" & "ghs-uninstall.bat" & Chr(34), 0, true
	Wscript.Sleep 500
End If

'environment
Dim strPath
strPath = CStr(objEnv("PATH"))

Dim iLen, iPos, iPos1
iLen = Len(strPath)
iPos = 1
Do
	iPos = InStr(iPos, strPath, Chr(34) & strInstallDir, 1)
	If iPos <> 0 Then
		iPos1 = InStr(iPos, strPath, ";", 1)
		If iPos1 = 0 Then
			iPos1 = iLen + 1
		End If
		If iPos <> 1 Then
			If Mid(strPath, iPos - 1, 1) = ";" Then
				iPos = iPos - 1
			End If
		End If
		strPath = Left(strPath, iPos - 1) & Right(strPath, iLen + 1 - iPos1)
		iLen = Len(strPath)
	End If
Loop While iPos <> 0
objEnv("PATH") = strPath

objEnv.Remove(strEnvSystemRoot)
objEnv.Remove(strEnvLws)
objEnv.Remove(strEnvUws)

'folders
fso.DeleteFolder strInstallDir, True
If iUninstallMode = 1 Then
	fso.DeleteFolder strLwsDir, True
	fso.DeleteFolder strUwsDir, True
	iPos = InStrRev(strInstallDir, "/", -1, 1)
	If iPos <> 0 Then
		strTemp = Left(strInstallDir, iPos - 1)
		fso.DeleteFolder strTemp, True
	End If
End If

Set objEnv = Nothing
Set objWsh = Nothing
Set fso = Nothing

'done
WScript.Echo "Done" & vbCrLf
